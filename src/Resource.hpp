#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

/* 
   base64.cpp and base64.h
   base64 encoding and decoding with C++.
Version: 1.01.00
Copyright (C) 2004-2017 René Nyffenegger
This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
1. The origin of this source code must not be misrepresented; you must not
claim that you wrote the original source code. If you use this source code
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original source code.
3. This notice may not be removed or altered from any source distribution.
René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string) {
	static const std::string base64_chars = 
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";
	size_t in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]) & 0xff;

			char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = 0; j < i; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]) & 0xff;

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

constexpr const char *kWallPngBase64 = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAASFBMVEVuRzRfPy2Gem58UDaEVDprRy91SjdfPS2Ug3NrRi9gPiqgj32WXUOOWT58TjaBTTl5SjVqQi58TTZtRDBqQC5dOSmJUz3///9mXiHsAAAAAWJLR0QXC9aYjwAAAAd0SU1FB+MMAQcdBasBx/4AAABxSURBVBjTZc7tDoMgEETRYUFZRSz90Pd/VJetY016/kBIyFwAQWJKKcZBRgN/cIOgyznr5Ga1e0YpRcuXn1hMras8TGtPQXWrNGcPL/fWj9sU9//9xG8/iOUkXPtnD7jPnv+O3fQ59oD77AH32cMO0gOZ/gnd9OE/RgAAACV0RVh0ZGF0ZTpjcmVhdGUAMjAxOS0xMS0zMFQxNzozNTozNSswMDowMHYNn7oAAAAldEVYdGRhdGU6bW9kaWZ5ADIwMTktMTEtMzBUMTU6MTk6MDUrMDA6MDCr08zhAAAAAElFTkSuQmCC";
constexpr const char *kEmptyPngBase64 = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAV1BMVEWSjoqYk46jn52loZ+cl5SZlI+fmpegnJqQi4ablZGKhoKTj4uNiIOJhYGdmJWVkY2alJCkoJ6PioWHg3+oo6CWko6qpqSIhICfm5mvqqeppaOtqaf///+fp5JyAAAAAWJLR0QcnARBBwAAAAd0SU1FB+MMAQcdDjzTHnYAAAC4SURBVBjTFY/bksMwDEIRli0ran1JnTTt/v9/bvrGDMwBACETNRdLqWY4tqzxwHNr0CzdM1h0dG2eZqI3J/YtvzSvY4z8gFfAThrPO/5WRC8L0wpRq7NN0lzgKvvQWn92SL4wk3rU+ujdyA8L3hJZ6TmuGth7xb3qlKOJ3qzvItGL4yUNOezS2J/gGsvuAk8iNPvDD3jo8Fu0bmogi5kUcn0ncS3UPmJHcxHFHlHvv2WK91BzSzT9B1SwCHqjbjCfAAAAJXRFWHRkYXRlOmNyZWF0ZQAyMDE5LTExLTMwVDE3OjM1OjM1KzAwOjAwdg2fugAAACV0RVh0ZGF0ZTptb2RpZnkAMjAxOS0xMS0zMFQxNToxODo0NCswMDowMGYsopEAAAAASUVORK5CYII=";
constexpr const char *kBoxPngBase64 = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAArlBMVEU7ODVCQTtAPzlCQDs+Ojg/PDk/PjlDQjxKEAA1NDA1MjA4NzI2MzA2NjA2NDA4ODI1MzA2NTA0Mi89DQAzMi4zMS5NRjVXTz1ZU0E+OzhxaFOEeWZ7cV9pYE9nX09zaVeJfWk0MS9eVkR7cV1gWkhkXEyUh3BqYVBhW0lgWEhmXkxsY1JYUD55bltCQjs3NjFxZ1OIfWhzaleRhW9rYlE/PzmDd2NWTjw6NzT///8IXHbeAAAAAWJLR0Q51wCVQAAAAAd0SU1FB+MMAQccFjakt2EAAADZSURBVBjTJY9tc4IwEIQvCQRrqBCUO70mqJCotUW09O3//7Ji3dn9srOzMw+AEFKoRKRS6zRLAdLZ09zkz4vClLZSyxUIWyMR4mRca2lhUyG/OE/N1vMOkz1oi0zE1DrnuqAt6BIpcnNo6S5ZgjTr48md3Gt7fotBG0gKfBT47mNICxA5cugaN930hJcFbAaklrn3V08xJMO0oC72N3f+GB0FkUNm8DjywX2OW4+kDHzV6JmJr9/+J6Keg1IVhkA0JQSVWRBLqZX4p5WJmP2C3d9phwftUK/+ABxlFk2jMc/8AAAAJXRFWHRkYXRlOmNyZWF0ZQAyMDE5LTExLTMwVDE3OjM1OjM1KzAwOjAwdg2fugAAACV0RVh0ZGF0ZTptb2RpZnkAMjAxOS0xMS0zMFQxNToxODoxNSswMDowMIi7p0EAAAAASUVORK5CYII=";
constexpr const char *kBoxTargetPngBase64 = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAsVBMVEU7ODVCQTtAPzlCQDs+Ojg/PDk/PjlDQjzCLQQ1NDA1MjA4NzI2MzA3NjE2NDA4ODI1MzA2NTA0Mi+6KwMzMi4zMS5yYTGCbjiHcTs+Oziqk025pm6zn2ChiEafh0atlVO8qnQ0MS+Odz2ynV6UfkCagkTDtIWijEiUf0GTez+PeD2dhUSmjkmFbzmhiUexm1tCQjuqkUu7qnOslVLBsYGljUg/Pzm3o2mBazc6NzQGAwDbGnDZAAAAAWJLR0Q6TgnE+gAAAAlwSFlzAAALEwAACxMBAJqcGAAAAAd0SU1FB+MMAQccI2AXc0IAAAAQY2FOdgAAABAAAADgAAAAAAAAAADdBwCdAAAA2UlEQVQY002PW1ODMBSETxISagOFtOWc9hiohaRXBat4+/9/TNQXd3ZfdnZ25gMQQgqVCC2N0akG0LO7uc3yRWFLt1SrNQhXIRHiZNwY6WBbId/7mppdzQ+Y7ME4ZCKm1nvfBePAlEiRm0NLP5IlSLs5nvzJny/XxxiMhaTAvwKf+hh0ASJDDl3jp5uB8HkB2xypZR7qW00xJPm0oC4OL/76OnoKIoPU4nHkg38bdz2SsvBeYc9MfPvoPyOaOSi1xBCIpoSgUgdiJY0Sv7QyEbMvcPt/tHm1/gYkXxYsFKwfmgAAACV0RVh0ZGF0ZTpjcmVhdGUAMjAxOS0xMS0zMFQxNzozNTozNSswMDowMHYNn7oAAAAldEVYdGRhdGU6bW9kaWZ5ADIwMTktMTEtMzBUMTU6MTg6MzUrMDA6MDDKnqA8AAAAAElFTkSuQmCC";
constexpr const char *kTargetPngBase64 = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAYFBMVEWSjoqYk46jn52loZ+cl5SZlI+fmpegnJqQi4ablZGKhoKTj4uNiIOJhYGdmJWVkY2Ihn+mKACAfnd5HQCPioXuOAHJLwGoo6CIhICkoJ6fm5mvqqeppaOtqaealJD///93qXAmAAAAAWJLR0QfBQ0QvQAAAAd0SU1FB+MMAQcdF1i4trYAAAC3SURBVBjTJY9BdsMwCEQHjIQIjYQlt4rdtPc/Zm13A4v/H8MAxLywpKzLUhIMjyT+geejQhI1S+Aca5NqPQZbNcbWP7/WMe8JK4D2dXCMRK+I0fLErpn7Sfl2jGBCMaSUCzulA/ty8RKXxN+c8SJPEWfIURxbKzi/inVcVub3ZEbLJ6WK5D3Etyd4/mfEtVR/4KX0Iffh2lQUzFmVMo/53hnHRGm/vqEakWBzL2ffvJM1FzVdWOUP9tMLfatwg6wAAAAldEVYdGRhdGU6Y3JlYXRlADIwMTktMTEtMzBUMTc6MzU6MzUrMDA6MDB2DZ+6AAAAJXRFWHRkYXRlOm1vZGlmeQAyMDE5LTExLTMwVDE2OjMyOjQ2KzAwOjAwAzf2fgAAAABJRU5ErkJggg==";
constexpr const char *kPlayerPngBase64 = "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";
constexpr const char *kBackgroundPngBase64 = "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QAJgA5ABphGka+AAAACXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH3QsTAiYcg1NnNAAAASRJREFUOMuFkl1OwzAQhL86ic0iQiuQemkuwyW4TJGqppHY2PkxD8VuKqp6X+3ZmZ2ZzdfnRwToz0fa1zcAglfujXVCdzog0qLaI9Ji0mMCA9S1BUC1xzpBtc+Lt7s9ACIt1sl1QZp5nokxZobgNTNaJ1lJY58uZAAxLkzjiKlq5inclZ7AAMsyY0xF8IoJXhmDJ8aFeQpUtWWzMVnqGhi80p0OGQxcT0gSq6qise4G3J0OBK83Z6UxCSzS/rEMxBjzp7UHIi3D8IN1wjh6VHvqxJKcBjh33/+Ytrs9Yxh43b4TvNI0jqZxFw/WsSQm1Z5p9NnA/nzMzq/HPMr5+WVHqSemlHOpJ3UpZ+vkYU9MKedST0wp51JPTCnnUk/qUs6lnvwC9X0IFhjsRiUAAAAASUVORK5CYII=";

#endif
