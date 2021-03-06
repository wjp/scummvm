/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "xeen/worldofxeen/worldofxeen_cutscenes.h"
#include "xeen/sound.h"
#include "xeen/xeen.h"

namespace Xeen {
namespace WorldOfXeen {

void WorldOfXeenCutscenes::showWorldOfXeenEnding() {
	worldEnding1();
	if (!_vm->shouldQuit())
		worldEnding2();
	if (!_vm->shouldQuit())
		worldEnding3();
	if (!_vm->shouldQuit())
		worldEnding4();
}

void WorldOfXeenCutscenes::worldEnding1() {
	EventsManager &events = *_vm->_events;
	FileManager &files = *_vm->_files;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;
	Windows &windows = *_vm->_windows;
	Window &w0 = windows[0];
	Graphics::ManagedSurface savedBg(SCREEN_WIDTH, SCREEN_HEIGHT);

	files.setGameCc(1);
	sound.playSong("outday3.m");
	showPharaohEndText(Res.WORLD_END_TEXT[0], nullptr, nullptr);
	sound.playSound("elect.voc");

	screen.loadBackground("skymain.raw");
	savedBg.blitFrom(screen);
	screen.loadBackground("twrsky1.raw");
	screen.loadPage(0);
	screen.loadPage(1);
	screen.loadPalette("skymain.pal");

	SpriteResource sc02("sc02.eg2"), tower1("tower1.eg2"), tower2("tower2.eg2"),
		sc3a("sc3a.eg2"), sc06("sc06.eg2"), sc14("sc14.eg2"), sc13("sc13.eg2"),
		sc17("sc17.eg2"), cube("cube.eg2"), hands("hands.eg2"),
		sc10("sc10.eg2"), staff("staff.eg2");
	SpriteResource sc3b[2] = {
		SpriteResource("sc3b1.eg2"), SpriteResource("sc3b2.eg2")
	};
	SpriteResource sc20[4] = {
		SpriteResource("sc20[0].eg2"), SpriteResource("sc20b.eg2"),
		SpriteResource("sc20c.eg2"), SpriteResource("sc20d.eg2")
	};
	SpriteResource sc22[2] = {
		SpriteResource("sc22a.eg2"), SpriteResource("sc22b.eg2")
	};

	windows[41].writeString("\x1\xD");
	screen.fadeOut();
	while (!_vm->shouldQuit() && sound.isPlaying())
		events.pollEventsAndWait();

	sound.playSong("nwblksmt.m");
	screen.blitFrom(savedBg);
	setWorldEndingSubtitle(Res.WORLD_END_TEXT[1]);
	w0.update();
	screen.fadeIn();

	events.updateGameCounter();
	events.wait(60);

	for (int idx = 0; idx < 50 && !_vm->shouldQuit(); ++idx) {
		if (idx == 9 || idx == 22 || idx == 33 || idx == 44)
			sound.playSound("whoosh.voc");

		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc02.draw(0, idx);
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[1]);
		w0.update();

		events.wait(2);
	}

	for (int idx = 0; idx < 40 && !_vm->shouldQuit(); ++idx) {
		screen.horizMerge();
		events.updateGameCounter();
		tower1.draw(0, 0, Common::Point(0, 0), SPRFLAG_800);
		sc3a.draw(0, idx, Common::Point(91, 86), SPRFLAG_800);

		w0.update();
		events.wait(2);
	}

	int frame = 40, frame2 = 0;
	for (int idx = 0, xp = 0; idx < SCREEN_WIDTH && !_vm->shouldQuit(); ++idx) {
		events.updateGameCounter();
		screen.horizMerge(xp);
		tower1.draw(0, 0, Common::Point(idx, 0), SPRFLAG_800);
		sc3a.draw(0, frame, Common::Point(idx + 91, 86), SPRFLAG_800);
		tower2.draw(0, 0, Common::Point(idx - 320, 0), SPRFLAG_800);
		tower2.draw(0, 1, Common::Point(idx - (SCREEN_WIDTH / 2), 0), SPRFLAG_800);
		sc3b[frame2 / 30].draw(0, frame2 % 30, Common::Point(idx - 277, 65), SPRFLAG_800);

		frame = (frame + 1) % 57;
		if (idx > (SCREEN_WIDTH / 2))
			frame2 = (frame2 + 1) % 60;
		xp -= 2;
		if (xp < 1)
			xp = SCREEN_WIDTH;

		if (idx > 120)
			setWorldEndingSubtitle(Res.WORLD_END_TEXT[2]);
		w0.update();
		events.wait(2);
	}

	for (; frame2 < 60 && !_vm->shouldQuit(); ++frame2) {
		events.updateGameCounter();
		screen.horizMerge(frame);
		tower2.draw(0, 0, Common::Point(0, 0), SPRFLAG_800);
		tower2.draw(0, 1, Common::Point(SCREEN_WIDTH / 2, 0), SPRFLAG_800);
		sc3b[frame2 / 30].draw(frame2 % 30, 0, Common::Point(43, 65), SPRFLAG_800);

		setWorldEndingSubtitle(Res.WORLD_END_TEXT[2]);
		w0.update();
		events.wait(2);
	}

	if (_vm->shouldQuit())
		return;
	screen.freePages();
	screen.fadeOut();
	screen.loadBackground("foura.raw");
	savedBg.blitFrom(screen);

	sc06.draw(0, 0, Common::Point(26, 25));
	w0.update();
	screen.fadeIn();

	for (int idx = 0; idx < 26 && !_vm->shouldQuit(); ++idx) {
		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc06.draw(0, idx, Common::Point(26, 75));
		events.wait(2);
	}

	if (_vm->shouldQuit())
		return;
	screen.fadeOut();
	screen.loadBackground("eg140001.raw");
	savedBg.blitFrom(screen);
	w0.update();
	setWorldEndingSubtitle(Res.WORLD_END_TEXT[3]);
	screen.fadeIn();

	for (int idx1 = 0; idx1 < 2 && !_vm->shouldQuit(); ++idx1) {
		for (int idx2 = 0; idx2 < 15 && !_vm->shouldQuit(); ++idx2) {
			events.updateGameCounter();
			screen.blitFrom(savedBg);
			sc14.draw(0, idx2, Common::Point(141, 63));
			setWorldEndingSubtitle(Res.WORLD_END_TEXT[3]);

			w0.update();
			events.wait(2);
		}
	}

	screen.loadBackground("eg100001.raw");
	screen.loadPage(0);
	savedBg.blitFrom(screen);

	for (int idx1 = 0; idx1 < 2 && !_vm->shouldQuit(); ++idx1) {
		for (int idx2 = 0; idx2 < 6 && !_vm->shouldQuit(); ++idx2) {
			events.updateGameCounter();
			screen.blitFrom(savedBg);
			sc14.draw(0, idx2, Common::Point(26, 21));
			setWorldEndingSubtitle(Res.WORLD_END_TEXT[4]);

			w0.update();
			events.wait(2);
		}
	}

	screen.blitFrom(savedBg);
	sc13.draw(0, 5, Common::Point(26, 21));
	savedBg.blitFrom(screen);
	setWorldEndingSubtitle(Res.WORLD_END_TEXT[4]);
	w0.update();

	frame = 0;
	for (int idx = 185; idx > 68 && !_vm->shouldQuit(); idx -= 3) {
		if (!sound.isPlaying())
			sound.playSound("cast.voc");

		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc17.draw(0, 0, Common::Point(33, idx), SPRFLAG_4000);
		sc17.draw(0, frame, Common::Point(33, idx), SPRFLAG_4000);
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[4]);

		w0.update();
		events.wait(2);
		frame = (frame + 1) % 17;
	}

	for (int idx = 0; idx < 17 && !_vm->shouldQuit(); ++idx) {
		if (!sound.isPlaying())
			sound.playSound("cast.voc");

		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc17.draw(0, 0, Common::Point(33, 68), SPRFLAG_4000);
		sc17.draw(0, idx, Common::Point(33, 68), SPRFLAG_4000);
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[4]);

		w0.update();
		events.wait(2);
	}

	if (_vm->shouldQuit())
		return;
	screen.blitFrom(savedBg);
	sc17.draw(0, 0, Common::Point(33, 68), SPRFLAG_4000);
	setWorldEndingSubtitle(Res.WORLD_END_TEXT[4]);
	w0.update();
	screen.fadeOut();

	screen.loadBackground("eg140001.raw");
	savedBg.blitFrom(screen);
	w0.update();
	screen.fadeIn();

	for (int idx1 = 0; idx1 < 2 && !_vm->shouldQuit(); ++idx1) {
		for (int idx2 = 0; idx2 < 15 && !_vm->shouldQuit(); ++idx2) {
			events.updateGameCounter();
			screen.blitFrom(savedBg);
			sc14.draw(0, idx2, Common::Point(141, 63));

			w0.update();
			events.wait(3);
		}
	}

	screen.horizMerge(0);
	savedBg.blitFrom(screen);

	for (int idx1 = 0; idx1 < 3 && !_vm->shouldQuit(); ++idx1) {
		for (int idx2 = 0; idx2 < 15 && !_vm->shouldQuit(); ++idx2) {
			events.updateGameCounter();
			screen.blitFrom(savedBg);
			sc10.draw(0, idx2, Common::Point(26, 21));
			setWorldEndingSubtitle(Res.WORLD_END_TEXT[5]);

			w0.update();
			events.wait(3);
		}
	}

	screen.blitFrom(savedBg);
	setWorldEndingSubtitle(Res.WORLD_END_TEXT[5]);
	w0.update();

	for (int idx = 185; idx > 13 && !_vm->shouldQuit(); idx -= 6) {
		events.updateGameCounter();
		screen.blitFrom(savedBg);
		staff.draw(0, 0, Common::Point(196, idx), SPRFLAG_4000);
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[5]);

		w0.update();
		events.wait(2);
	}

	events.updateGameCounter();
	events.wait(30);
	if (_vm->shouldQuit())
		return;

	screen.fadeOut();
	screen.loadBackground("eg140001.raw");
	savedBg.blitFrom(screen);
	w0.update();
	screen.fadeIn();

	for (int idx1 = 0; idx1 < 2 && !_vm->shouldQuit(); ++idx1) {
		for (int idx2 = 0; idx2 < 15 && !_vm->shouldQuit(); ++idx2) {
			events.updateGameCounter();
			screen.blitFrom(savedBg);
			sc14.draw(0, idx2, Common::Point(141, 63));

			w0.update();
			events.wait(3);
		}
	}

	if (_vm->shouldQuit())
		return;
	screen.loadBackground("tablmain.raw");
	savedBg.blitFrom(screen);
	screen.loadPage(1);
	sc20[0].draw(0, 0, Common::Point(26, 55));
	hands.draw(0, 0, Common::Point(58, 17));
	cube.draw(0, 0, Common::Point(101, 11), SPRFLAG_4000);
	w0.update();
	sound.playSound("cast.voc");

	for (int idx = 0; idx < 5 && !_vm->shouldQuit(); ++idx) {
		if (!sound.isPlaying())
			sound.playSound("cast.voc");

		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc20[0].draw(0, 0, Common::Point(26, 55));
		hands.draw(0, 0, Common::Point(58, 17));
		cube.draw(0, 0, Common::Point(101, 11), SPRFLAG_4000);
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[6]);

		w0.update();
		events.wait(2);
	}

	frame = 0;
	for (int idx = 0; idx < 16 && !_vm->shouldQuit(); ++idx) {
		if (!sound.isPlaying())
			sound.playSound("cast.voc");

		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc20[0].draw(0, 0, Common::Point(26, 55));
		hands.draw(0, 0, Common::Point(58, 17));
		cube.draw(0, frame, Common::Point(101, 11), SPRFLAG_4000);
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[6]);

		w0.update();
		events.wait(2);
		frame = (frame + 1) % 5;
	}

	frame = 0;
	for (int idx = 11; idx < 82 && !_vm->shouldQuit(); ++idx) {
		if (!sound.isPlaying())
			sound.playSound("cast.voc");

		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc20[0].draw(0, 0, Common::Point(26, 55));
		cube.draw(0, frame, Common::Point(101, idx), SPRFLAG_4000);
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[6]);

		w0.update();
		events.wait(2);
		frame = (frame + 1) % 5;
	}

	sound.stopSound();
	if (_vm->shouldQuit())
		return;

	sound.playSound("click.voc");
	sound.playSound("padspell.voc");

	for (int idx = 0; idx < 14 && !_vm->shouldQuit(); ++idx) {
		if (idx == 10)
			sound.playSound("padspell.voc");

		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc20[idx / 7].draw(0, idx % 7, Common::Point(26, 55));
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[6]);

		w0.update();
		events.wait(2);
	}

	for (int idx = 0; idx < 17 && !_vm->shouldQuit(); ++idx) {
		if (idx == 10)
			sound.playSound("padspell.voc");

		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc20[2].draw(0, idx, Common::Point(26, 55));
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[6]);

		w0.update();
		events.wait(3);
	}

	for (int idx = 0; idx < 6 && !_vm->shouldQuit(); ++idx) {
		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc20[3].draw(0, idx, Common::Point(26, 55));
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[6]);

		w0.update();
		events.wait(4);
	}

	if (_vm->shouldQuit())
		return;
	screen.blitFrom(savedBg);
	setWorldEndingSubtitle(Res.WORLD_END_TEXT[6]);
	w0.update();
	events.updateGameCounter();
	events.wait(30);

	screen.horizMerge(SCREEN_WIDTH);
	savedBg.blitFrom(screen);
	screen.freePages();

	for (int idx = 0; idx < 35 && !_vm->shouldQuit(); ++idx) {
		if (idx == 3)
			sound.playSound("click.voc");
		else if (idx == 5 || idx == 15)
			sound.playSound("elect.voc");
		else if (idx == 22)
			sound.playSound("explosio.voc");

		events.updateGameCounter();
		screen.blitFrom(savedBg);
		sc22[idx / 20].draw(0, idx % 20, Common::Point(112, 17));
		setWorldEndingSubtitle(Res.WORLD_END_TEXT[7]);

		w0.update();
		events.wait(3);
	}
}

void WorldOfXeenCutscenes::worldEnding2() {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;
	Windows &windows = *_vm->_windows;
	Window &w0 = windows[0];
	Graphics::ManagedSurface savedBg(SCREEN_WIDTH, SCREEN_HEIGHT);

	SpriteResource sc23[8] = {
		SpriteResource("sc23a.eg2"), SpriteResource("sc23b.eg2"),
		SpriteResource("sc23c.eg2"), SpriteResource("sc23d.eg2"),
		SpriteResource("sc23e.eg2"), SpriteResource("sc23f.eg2"),
		SpriteResource("sc23g.eg2"), SpriteResource("sc23h.eg2")
	};
	SpriteResource sc24("sc24.eg2");

	screen.fadeOut();
	screen.loadBackground("eg23prt2.raw");
	savedBg.blitFrom(screen);
	sc23[0].draw(0, 0);
	w0.update();
	screen.fadeIn();

	int frame = 0;
	for (int idx = 0; idx < 61 && !_vm->shouldQuit(); ++idx) {
		if (idx == 2 || idx == 15 || idx == 25 || idx == 33 || idx == 41)
			sound.playSound("gascompr.voc");

		events.updateGameCounter();
		sc23[idx / 8].draw(0, frame % 8);
		w0.update();
		events.wait(4);
	}
}

void WorldOfXeenCutscenes::worldEnding3() {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;
	Windows &windows = *_vm->_windows;
	Window &w0 = windows[0];
	Graphics::ManagedSurface savedBg(SCREEN_WIDTH, SCREEN_HEIGHT);

	SpriteResource sc25("sc25.eg2"), sc262("sc262.eg2"), sc263("sc263.eg2"),
		sc264("sc264.eg2");
	SpriteResource sc261[2] = {
		SpriteResource("sc261a.eg2"), SpriteResource("sc261b.eg2")
	};

	screen.fadeOut();
	screen.loadBackground("eg250001.raw");
	screen.loadPalette("eg250001.pal");
	w0.update();
	screen.fadeIn();
	sound.playSound("comet.voc");

	for (int idx = 0; idx < 52 && !_vm->shouldQuit(); ++idx) {
		if (idx == 28)
			sound.playSound("click.voc");
		if (!sound.isPlaying())
			sound.playSound("comet.voc");

		events.updateGameCounter();
		sc25.draw(0, idx);
		w0.update();
		events.wait(2);
	}
	sound.stopSound();

	screen.loadBackground("blank.raw");
	screen.loadPalette("skymain.pal");
	sc261[0].draw(0, 0, Common::Point(7, 4));
	sc262.draw(0, 0, Common::Point(86, 4));
	sc263.draw(0, 0, Common::Point(164, 4));
	sc264.draw(0, 0, Common::Point(242, 4));
	w0.update();
	screen.fadeIn(0x81);

	int frame1 = 0, frame2 = 0, frame3 = 0, ctr = 0;
	for (int idx = 0; idx < 78 && !_vm->shouldQuit(); ++idx) {
		events.updateGameCounter();
		sc261[ctr / 14].draw(0, idx % 17, Common::Point(7, 4));
		sc262.draw(0, frame1, Common::Point(86, 4));
		sc263.draw(0, frame2, Common::Point(164, 4));
		sc264.draw(0, frame3, Common::Point(242, 4));

		if (idx == 10 || idx == 28 || idx == 43 || idx == 56)
			sound.playSound("photon.voc");

		ctr = (ctr + 1) % 28;
		frame1 = (frame1 + 1) % 9;
		frame2 = (frame2 + 1) % 19;
		frame3 = (frame3 + 1) % 10;

		if (idx > 12 && ctr < 13)
			ctr = 13;
		if (idx < 23)
			frame1 = 0;
		else if (idx > 26 && frame1 < 5)
			frame1 = 5;

		if (idx < 34)
			frame2 = 0;
		else if (idx > 43 && frame2 < 12)
			frame2 = 12;

		if (idx < 53)
			frame3 = 0;
		else if (idx > 57 && frame3 < 5)
			frame3 = 5;

		w0.update();
		events.wait(2);
	}
}

void WorldOfXeenCutscenes::worldEnding4() {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;
	Windows &windows = *_vm->_windows;
	Window &w0 = windows[0];
	Graphics::ManagedSurface savedBg(SCREEN_WIDTH, SCREEN_HEIGHT);

	SpriteResource sc27("sc27.eg2");

	screen.fadeOut();
	screen.loadBackground("eg270001.raw");
	screen.loadPalette("eg250001.pal");
	screen.fadeIn();

	for (int idx = 0; idx < 89 && !_vm->shouldQuit(); ++idx) {
		if (!sound.isPlaying())
			sound.playSound("comet.voc");
		if (idx == 19 || idx == 60)
			sound.playSound("click.voc");

		events.updateGameCounter();
		sc27.draw(0, idx);
		w0.update();
		events.wait(2);
	}

	if (_vm->shouldQuit())
		return;
	sound.stopSound();
	screen.fadeOut();


	// TODO
}

void WorldOfXeenCutscenes::setWorldEndingSubtitle(const Common::String &msg) {
	Windows &windows = *_vm->_windows;
	const char *const FORMAT1 = "\xB""000\t000\xC""38\x3""c%s";
	const char *const FORMAT2 = "\xB""000\t000\xC""39\x3""c%s";
	windows[28].writeString(Common::String::format(FORMAT1, msg.c_str()));
	windows[28].writeString(Common::String::format(FORMAT2, msg.c_str()));
}

} // End of namespace WorldOfXeen
} // End of namespace Xeen
