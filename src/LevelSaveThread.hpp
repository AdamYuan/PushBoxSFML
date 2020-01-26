#ifndef LEVEL_SAVE_THREAD_HPP
#define LEVEL_SAVE_THREAD_HPP

#include <optional>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <string>
#include "DataBase.hpp"
#include "Error.hpp"

class LevelSaveThread {
	private:
		struct SaveInfo {
			int m_id, m_width, m_height;
			std::string m_str;
		};
		std::condition_variable m_done_cv;
		std::mutex m_task_mutex, m_cv_mutex;
		std::thread m_thread; 
		std::atomic_bool m_running = true;
		std::optional<SaveInfo> m_task;

		DataBase &m_db;

		void thread_func() {
			std::optional<SaveInfo> info;

			while(true) {
				if(m_task_mutex.try_lock()) { //fetch task
					if(m_task.has_value()) {
						info = std::move(m_task);
						m_task.reset();

						m_task_mutex.unlock();
					}
					else { //all work is done
						m_task_mutex.unlock();

						m_done_cv.notify_all();
						info.reset();
						if(!m_running)
							return;
					}
				}

				if(info.has_value()) {
					m_db.SaveLevel(info->m_width, info->m_height, info->m_str.c_str(), info->m_id);
					info.reset();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
	public:
		inline LevelSaveThread(DataBase &db) : m_db{db} {
			m_thread = std::thread(&LevelSaveThread::thread_func, this);
		}
		inline void Enqueue(const Level &level, int id) {
			std::string str; if(!level.ExportToStr(str)) return;

			std::lock_guard<std::mutex> guard{m_task_mutex};
			if(m_task.has_value() && m_task->m_id != id) pberr::push("Auto-save Thread ERROR");
			m_task = {id, level.Width(), level.Height(), std::move(str)};
		}
		inline void Done() {
			std::unique_lock<std::mutex> lk{m_cv_mutex};
			m_done_cv.wait(lk);
		}
		inline ~LevelSaveThread() {
			m_running = false;
			m_thread.join();
		}
};

#endif
