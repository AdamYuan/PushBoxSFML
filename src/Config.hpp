#ifndef CONFIG_HPP
#define CONFIG_HPP

constexpr const char *kDataBaseFilename = "level.db";
constexpr int kBlockSize = 32, kMaxLevelWidth = 15, kMaxLevelHeight = 14, kSmallBlockSize = 24;
constexpr float kSpriteScale = (kBlockSize / 16.0f);
constexpr int kUiHeight = kBlockSize, 
		  kWindowWidth = kMaxLevelWidth * kBlockSize, kWindowHeight = (kMaxLevelHeight) * kBlockSize + kUiHeight;
constexpr int kEditorHoverAlpha = 128;
constexpr float kErrorNotificationLifespan = 3.0f;
constexpr int kUiItemWidth = 100;
constexpr float kLightRadius = 128.0f;
constexpr int kLightCircleDivision = 18;
constexpr float kLightAngle = 3.14159265358979323846f * 2.0f / (float)kLightCircleDivision;

struct ConstLevelEntry { int width, height; const char *str; };
constexpr ConstLevelEntry kNewLevel = {
	10, 
	10, "P........."
		".........."
		".........."
		".........."
		".........."
		".........."
		".........."
		".........."
		".........."
		".........."
};
constexpr ConstLevelEntry kDefaultLevels[] = {
	{
		8, 
		8,  "..###..."
			"..#T#..."
			"..#.####"
			"###B.BT#"
			"#T.BP###"
			"####B#.."
			"...#T#.."
			"...###.."
	},
	{
		9, 
		9,  "#####...."
			"#..P#...."
			"#.BB#.###"
			"#.B.#.#T#"
			"###.###T#"
			".##....T#"
			".#...#..#"
			".#...####"
			".#####..."
	},
	{
		10, 
		7,  ".#######.."
			".#.....###"
			"##B###...#"
			"#.P.B..B.#"
			"#.TT#.B.##"
			"##TT#...#."
			".########."
	},
	{
		6,  
		8,  ".####."
			"##..#."
			"#.PB#."
			"##B.##"
			"##.B.#"
			"#TB..#"
			"#TTbT#"
			"######"
	},
	{
		8, 
		8,  ".#####.."
			".#P.###."
			".#.B..#."
			"###.#.##"
			"#T#.#..#"
			"#TB..#.#"
			"#T...B.#"
			"########"
	},
	{
		10, 
		8,  "...#######"
			"..##..#.P#"
			"..#...#..#"
			"..#B.B.B.#"
			"..#.B##..#"
			"###.B.#.##"
			"#TTTTT..#."
			"#########."
	},
	{
		10, 
		7,  "...######."
			".###....#."
			"##T.B##.##"
			"#TTB.B..P#"
			"#TT.B.B.##"
			"######..#."
			".....####."
	},
	{
		8, 
		7,  "..######"
			"..#....#"
			"###BBB.#"
			"#P.BTT.#"
			"#.BTTT##"
			"####..#."
			"...####."
	},
	{
		8, 
		7,  "..#####."
			"###..P#."
			"#..BT.##"
			"#..TBT.#"
			"###.bB.#"
			"..#...##"
			"..#####."
	}
};


#endif
