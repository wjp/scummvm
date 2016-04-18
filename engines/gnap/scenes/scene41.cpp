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

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "gnap/gnap.h"
#include "gnap/gamesys.h"
#include "gnap/resource.h"

namespace Gnap {

enum {
	kHSPlatypus		= 0,
	kHSExitCave		= 1,
	kHSExit			= 2,
	kHSExitBBQ		= 3,
	kHSToyVendor	= 4,
	kHSKid			= 5,
	kHSToyUfo		= 6,
	kHSDevice		= 7,
	kHSWalkArea1	= 8
};

enum {
	kHSUfoExitLeft	= 1,
	kHSUfoExitRight	= 2,
	kHSUfoDevice	= 3,
	kHSUfoWalkArea1	= 4
};

enum {
	kASLeaveScene				= 0,
	kASUseQuarterWithToyVendor	= 1,
	kASTalkToyVendor			= 2,
	kASUseGumWithToyUfo			= 3,
	kASUseChickenBucketWithKid	= 4,
	kASGrabKid					= 5,
	kASGiveBackToyUfo			= 6,
	kASToyUfoLeaveScene			= 7,
	kASToyUfoRefresh			= 8,
	kASUfoGumAttached			= 9
};

int GnapEngine::scene41_init() {
	_gameSys->setAnimation(0, 0, 0);
	_gameSys->setAnimation(0, 0, 1);
	_gameSys->setAnimation(0, 0, 2);
	return 0x129;
}

void GnapEngine::scene41_updateHotspots() {
	if (isFlag(kGFGnapControlsToyUFO)) {
		setHotspot(kHSPlatypus, 0, 0, 0, 0, SF_DISABLED);
		setHotspot(kHSUfoExitLeft, 0, 0, 10, 500, SF_EXIT_L_CURSOR | SF_DISABLED);
		setHotspot(kHSUfoExitRight, 790, 0, 799, 500, SF_EXIT_R_CURSOR);
		setHotspot(kHSUfoWalkArea1, 0, 0, 800, 470, SF_DISABLED);
		setDeviceHotspot(kHSUfoDevice, -1, -1, -1, -1);
		_hotspotsCount = 5;
	} else {
		setHotspot(kHSPlatypus, 0, 0, 0, 0, SF_WALKABLE | SF_TALK_CURSOR | SF_GRAB_CURSOR | SF_LOOK_CURSOR);
		setHotspot(kHSExitCave, 150, 590, 650, 600, SF_EXIT_D_CURSOR | SF_WALKABLE, 5, 9);
		setHotspot(kHSExit, 0, 100, 10, 599, SF_EXIT_L_CURSOR | SF_DISABLED, 0, 8);
		setHotspot(kHSExitBBQ, 790, 100, 799, 599, SF_EXIT_R_CURSOR | SF_WALKABLE, 10, 8);
		setHotspot(kHSToyVendor, 320, 150, 430, 310, SF_PLAT_CURSOR | SF_TALK_CURSOR | SF_GRAB_CURSOR | SF_LOOK_CURSOR);
		setHotspot(kHSKid, 615, 340, 710, 460, SF_PLAT_CURSOR | SF_TALK_CURSOR | SF_GRAB_CURSOR | SF_LOOK_CURSOR);
		setHotspot(kHSToyUfo, 0, 0, 0, 0, SF_GRAB_CURSOR);
		setHotspot(kHSWalkArea1, 0, 0, 800, 470);
		setDeviceHotspot(kHSDevice, -1, -1, -1, -1);
		_hotspotsCount = 9;
	}
}

void GnapEngine::scene41_run() {
	queueInsertDeviceIcon();

	if (isFlag(kGFGnapControlsToyUFO)) {
		_toyUfoX = 770;
		if (_toyUfoY < 0 || _toyUfoY > 300)
			_toyUfoY = 150;
		if (!_timers[9])
			_gnapActionStatus = kASGiveBackToyUfo;
	} else {
		if (!isFlag(kGFUnk16) && !isFlag(kGFJointTaken) && !isFlag(kGFUnk18) && !isFlag(kGFGroceryStoreHatTaken))
			toyUfoSetStatus(kGFUnk16);
		_toyUfoX = 600;
		_toyUfoY = 200;
	}

	_toyUfoId = 0;
	_toyUfoActionStatus = -1;
	_toyUfoSequenceId = toyUfoGetSequenceId();
	_toyUfoNextSequenceId = _toyUfoSequenceId;

	_gameSys->setAnimation(_toyUfoSequenceId | 0x10000, _toyUfoId, 2);
	_gameSys->insertSequence(_toyUfoSequenceId | 0x10000, _toyUfoId, 0, 0, kSeqNone, 0, _toyUfoX - 274, _toyUfoY - 128);
	_gameSys->insertSequence(0x128, 0, 0, 0, kSeqLoop, 0, 0, 0);
	
	if (isFlag(kGFGnapControlsToyUFO))
		_s41_currKidSequenceId = 0x11B;
	else
		_s41_currKidSequenceId = 0x11D;
	
	_s41_nextKidSequenceId = -1;
	
	_gameSys->setAnimation(_s41_currKidSequenceId, 1, 4);
	_gameSys->insertSequence(_s41_currKidSequenceId, 1, 0, 0, kSeqNone, 0, 0, 0);
	
	_s41_currToyVendorSequenceId = 0x118;
	_s41_nextToyVendorSequenceId = -1;
	
	_gameSys->setAnimation(0x118, 1, 3);
	_gameSys->insertSequence(_s41_currToyVendorSequenceId, 1, 0, 0, kSeqNone, 0, 0, 0);
	_gameSys->insertSequence(0x127, 2, 0, 0, kSeqNone, 0, 0, 0);
	
	if (isFlag(kGFGnapControlsToyUFO)) {
		_gnapSequenceId = 0x120;
		_gnapSequenceDatNum = 0;
		_gnapIdleFacing = kDirUpRight;
		_gnapX = 7;
		_gnapY = 7;
		_gnapId = 140;
		_gameSys->insertSequence(0x120, 140, 0, 0, kSeqNone, 0, 0, 0);
		_gameSys->setAnimation(makeRid(_gnapSequenceDatNum, _gnapSequenceId), _gnapId, 0);
		initPlatypusPos(8, 10, kDirBottomLeft);
		endSceneInit();
	} else if (_prevSceneNum == 45) {
		initGnapPos(-1, 8, kDirUpRight);
		initPlatypusPos(-2, 8, kDirUpLeft);
		endSceneInit();
		platypusWalkTo(1, 8, -1, 0x107C2, 1);
		gnapWalkTo(2, 8, -1, 0x107B9, 1);
	} else if (_prevSceneNum == 42) {
		initGnapPos(11, 8, kDirUpRight);
		initPlatypusPos(11, 9, kDirUpLeft);
		endSceneInit();
		gnapWalkTo(8, 8, -1, 0x107BA, 1);
		platypusWalkTo(9, 8, -1, 0x107D2, 1);
	} else {
		initGnapPos(5, 8, kDirBottomRight);
		initPlatypusPos(6, 8, kDirBottomLeft);
		endSceneInit();
	}

	_timers[4] = getRandom(100) + 100;
	_timers[5] = getRandom(30) + 20;

	while (!_sceneDone) {
		if (!isSoundPlaying(0x1094B))
			playSound(0x1094B, true);
	
		if (!isFlag(kGFGnapControlsToyUFO)) {
			_hotspots[kHSToyUfo]._x1 = _toyUfoX - 25;
			_hotspots[kHSToyUfo]._y1 = _toyUfoY - 20;
			_hotspots[kHSToyUfo]._x2 = _toyUfoX + 25;
			_hotspots[kHSToyUfo]._y2 = _toyUfoY + 20;
		}
	
		updateMouseCursor();
		updateCursorByHotspot();
	
		testWalk(0, 0, -1, -1, -1, -1);
	
		_sceneClickedHotspot = getClickedHotspotId();
		updateGrabCursorSprite(0, 0);
	
		if (isFlag(kGFGnapControlsToyUFO)) {

			switch (_sceneClickedHotspot) {

			case kHSUfoExitLeft:
				if (_toyUfoActionStatus < 0) {
					_isLeavingScene = true;
					_toyUfoActionStatus = kASToyUfoLeaveScene;
					_newSceneNum = 45;
					toyUfoFlyTo(-35, -1, -35, 799, 0, 300, 2);
				}
				break;
	
			case kHSUfoExitRight:
				if (_toyUfoActionStatus < 0) {
					_isLeavingScene = true;
					_toyUfoActionStatus = kASToyUfoLeaveScene;
					_newSceneNum = 42;
					toyUfoFlyTo(835, -1, 0, 835, 0, 300, 2);
				}
				break;
	
			case kHSUfoDevice:
				runMenu();
				scene41_updateHotspots();
				_timers[4] = getRandom(100) + 100;
				_timers[5] = getRandom(30) + 20;
				break;
		
			}

		} else {

			switch (_sceneClickedHotspot) {
	
			case kHSDevice:
				runMenu();
				scene41_updateHotspots();
				_timers[4] = getRandom(100) + 100;
				_timers[5] = getRandom(30) + 20;
				break;
	
			case kHSPlatypus:
				if (_gnapActionStatus < 0) {
					if (_grabCursorSpriteIndex >= 0) {
						playGnapImpossible(_platX, _platY);
					} else {
						switch (_verbCursor) {
						case LOOK_CURSOR:
							playGnapMoan1(_platX, _platY);
							break;
						case GRAB_CURSOR:
							gnapKissPlatypus(0);
							break;
						case TALK_CURSOR:
							playGnapBrainPulsating(_platX, _platY);
							playPlatypusSequence(getPlatypusSequenceId());
							break;
						case PLAT_CURSOR:
							playGnapImpossible(_platX, _platY);
							break;
						}
					}
				}
				break;
	
			case kHSExitCave:
				_isLeavingScene = true;
				gnapWalkTo(_hotspotsWalkPos[kHSExitCave].x, _hotspotsWalkPos[kHSExitCave].y, 0, 0x107AE, 1);
				_gnapActionStatus = kASLeaveScene;
				_newSceneNum = 40;
				break;
	
			case kHSExit:
				_isLeavingScene = true;
				gnapWalkTo(_hotspotsWalkPos[kHSExit].x, -1, 0, 0x107AF, 1);
				_gnapActionStatus = kASLeaveScene;
				platypusWalkTo(_hotspotsWalkPos[kHSExit].x, -1, -1, 0x107CF, 1);
				_newSceneNum = 45;
				break;
	
			case kHSExitBBQ:
				_isLeavingScene = true;
				gnapWalkTo(_hotspotsWalkPos[kHSExitBBQ].x, -1, 0, 0x107AB, 1);
				_gnapActionStatus = kASLeaveScene;
				platypusWalkTo(_hotspotsWalkPos[kHSExitBBQ].x, -1, -1, 0x107CD, 1);
				_newSceneNum = 42;
				break;
	
			case kHSToyVendor:
				if (_grabCursorSpriteIndex == kItemDiceQuarterHole) {
					_gnapActionStatus = kASUseQuarterWithToyVendor;
					gnapWalkTo(4, 7, 0, 0x107BB, 9);
					playGnapShowItem(_grabCursorSpriteIndex, 5, 0);
				} else if (_grabCursorSpriteIndex >= 0) {
					playGnapShowCurrItem(4, 7, 5, 0);
				} else {
					switch (_verbCursor) {
					case LOOK_CURSOR:
						playGnapMoan1(5, 0);
						break;
					case GRAB_CURSOR:
						playGnapImpossible(0, 0);
						break;
					case TALK_CURSOR:
						_gnapIdleFacing = kDirUpRight;
						gnapWalkTo(4, 7, 0, getGnapSequenceId(gskBrainPulsating, 0, 0) | 0x10000, 1);
						_gnapActionStatus = kASTalkToyVendor;
						break;
					case PLAT_CURSOR:
						playGnapImpossible(0, 0);
						break;
					}
				}
				break;
	
			case kHSKid:
				if (_grabCursorSpriteIndex == kItemChickenBucket) {
					gnapWalkTo(7, 7, 0, 0x107BB, 1);
					_gnapIdleFacing = kDirUpRight;
					_gnapActionStatus = kASUseChickenBucketWithKid;
				} else if (_grabCursorSpriteIndex >= 0) {
					playGnapShowCurrItem(7, 7, 8, 0);
				} else {
					switch (_verbCursor) {
					case LOOK_CURSOR:
						playGnapScratchingHead(9, 0);
						break;
					case GRAB_CURSOR:
						gnapWalkTo(7, 7, 0, 0x107BB, 1);
						_gnapIdleFacing = kDirUpRight;
						_gnapActionStatus = kASGrabKid;
						break;
					case TALK_CURSOR:
						_gnapIdleFacing = kDirUpRight;
						gnapWalkTo(7, 7, 0, getGnapSequenceId(gskBrainPulsating, 0, 0) | 0x10000, 1);
						break;
					case PLAT_CURSOR:
						playGnapImpossible(0, 0);
						break;
					}
				}
				break;
	
			case kHSToyUfo:
				if (_grabCursorSpriteIndex == kItemGum) {
					playGnapPullOutDevice(9, 0);
					_gameSys->setAnimation(makeRid(_gnapSequenceDatNum, _gnapSequenceId), _gnapId, 0);
					_gnapActionStatus = kASUseGumWithToyUfo;
				}
				break;
	
			case kHSWalkArea1:
				if (_gnapActionStatus < 0)
					gnapWalkTo(-1, -1, -1, -1, 1);
				break;
			}
		}
	
		if (isFlag(kGFGnapControlsToyUFO)) {
			if (!_timers[9] && _gnapActionStatus < 0) {
				_gnapActionStatus = kASGiveBackToyUfo;
				if (_gnapSequenceId == 0x121 || _gnapSequenceId == 0x122) {
					_gameSys->insertSequence(0x123, _gnapId, makeRid(_gnapSequenceDatNum, _gnapSequenceId), _gnapId, kSeqSyncWait, 0, 0, 0);
					_gnapSequenceId = 0x123;
					_gnapSequenceDatNum = 0;
					_gameSys->setAnimation(0x123, _gnapId, 0);
				}
			}
		}
	
		if (_mouseClickState._left && _gnapActionStatus < 0) {
			_mouseClickState._left = false;
			if (isFlag(kGFGnapControlsToyUFO)) {
				int sequenceId;
				if (_leftClickMouseX >= 400) {
					if (_gnapSequenceId == 0x11F || _gnapSequenceId == 0x120 || _gnapSequenceId == 0x123 || _gnapSequenceId == 0x126)
						sequenceId = 0x120;
					else if (_leftClickMouseX - _toyUfoX >= 400)
						sequenceId = 0x126;
					else
						sequenceId = 0x123;
				} else {
					if (_gnapSequenceId == 0x121 || _gnapSequenceId == 0x125 || _gnapSequenceId == 0x122)
						sequenceId = 0x122;
					else if (_toyUfoX - _leftClickMouseX >= 400)
						sequenceId = 0x125;
					else
						sequenceId = 0x121;
				}
				_gameSys->insertSequence(sequenceId, _gnapId, makeRid(_gnapSequenceDatNum, _gnapSequenceId), _gnapId, kSeqSyncWait, 0, 0, 0);
				_gnapSequenceId = sequenceId;
				_gnapSequenceDatNum = 0;
				_gameSys->setAnimation(sequenceId, _gnapId, 0);
				_toyUfoActionStatus = kASToyUfoRefresh;
				toyUfoFlyTo(-1, -1, 0, 799, 0, 300, 2);
			} else {
				gnapWalkTo(-1, -1, -1, -1, 1);
			}
		}

		scene41_updateAnimations();
	
		if (!_isLeavingScene) {
			if (_platypusActionStatus < 0)
				updatePlatypusIdleSequence();
			if (_gnapActionStatus < 0 && !isFlag(kGFGnapControlsToyUFO))
				updateGnapIdleSequence();
			if (!_timers[4]) {
				_timers[4] = getRandom(100) + 100;
				if (_gnapActionStatus < 0 && _platypusActionStatus < 0 && _toyUfoActionStatus == -1 && _s41_nextToyVendorSequenceId == -1) {
					switch (getRandom(3)) {
					case 0:
						_s41_nextToyVendorSequenceId = 0x113;
						break;
					case 1:
						_s41_nextToyVendorSequenceId = 0x117;
						break;
					case 2:
						_s41_nextToyVendorSequenceId = 0x119;
						break;
					}
					if (_s41_nextToyVendorSequenceId == _s41_currToyVendorSequenceId)
						_s41_nextToyVendorSequenceId = -1;
				}
			}
			if (!_timers[5]) {
				_timers[5] = getRandom(30) + 20;
				if (_gnapActionStatus < 0 && _platypusActionStatus < 0 && _toyUfoActionStatus == -1 && _s41_nextKidSequenceId == -1) {
					if (isFlag(kGFGnapControlsToyUFO))
						_s41_nextKidSequenceId = 0x11B;
					else if (getRandom(3) != 0)
						_s41_nextKidSequenceId = 0x11D;
					else
						_s41_nextKidSequenceId = 0x11E;
				}
			}
		}
	
		checkGameKeys();
	
		if (isKeyStatus1(8)) {
			clearKeyStatus1(8);
			runMenu();
			scene41_updateHotspots();
			_timers[4] = getRandom(100) + 100;
			_timers[5] = getRandom(30) + 20;
		}
		
		gameUpdateTick();
	
	}

}

void GnapEngine::scene41_updateAnimations() {

	if (_gameSys->getAnimationStatus(0) == 2) {
		switch (_gnapActionStatus) {
		case kASLeaveScene:
			_gameSys->setAnimation(0, 0, 0);
			_sceneDone = true;
			_gnapActionStatus = -1;
			break;
		case kASUseQuarterWithToyVendor:
			_gameSys->setAnimation(0, 0, 0);
			_s41_nextToyVendorSequenceId = 0x114;
			_gnapActionStatus = -1;
			break;
		case kASTalkToyVendor:
			_gameSys->setAnimation(0, 0, 0);
			_s41_nextToyVendorSequenceId = 0x116;
			_gnapActionStatus = -1;
			break;
		case kASUseGumWithToyUfo:
			_gameSys->setAnimation(0, 0, 0);
			playGnapUseDevice(9, 0);
			_gnapActionStatus = -1;
			setGrabCursorSprite(-1);
			invRemove(kItemGum);
			_toyUfoActionStatus = kASUfoGumAttached;
			break;
		case kASUseChickenBucketWithKid:
			if (_gameSys->getAnimationStatus(4) == 2) {
				_timers[2] = getRandom(30) + 20;
				_timers[3] = getRandom(50) + 200;
				setGrabCursorSprite(-1);
				_gameSys->insertSequence(0x11F, _gnapId, makeRid(_gnapSequenceDatNum, _gnapSequenceId), _gnapId, kSeqSyncWait, 0, 0, 0);
				_gnapSequenceId = 0x11F;
				_gnapSequenceDatNum = 0;
				_gameSys->setAnimation(0x11F, _gnapId, 0);
				_s41_nextKidSequenceId = 0x11A;
				_gameSys->insertSequence(0x11A, 1, _s41_currKidSequenceId, 1, kSeqSyncWait, 0, 0, 0);
				_gameSys->setAnimation(_s41_nextKidSequenceId, 1, 4);
				_s41_currKidSequenceId = _s41_nextKidSequenceId;
				_s41_nextKidSequenceId = 0x11B;
				_timers[5] = getRandom(30) + 20;
				_gnapActionStatus = -1;
				setFlag(kGFGnapControlsToyUFO);
				scene41_updateHotspots();
				_timers[9] = 600;
			}
			break;
		case kASGrabKid:
			if (_gameSys->getAnimationStatus(3) == 2 && _gameSys->getAnimationStatus(4) == 2) {
				_timers[2] = getRandom(30) + 20;
				_timers[3] = getRandom(50) + 200;
				_gameSys->insertSequence(0x110, _gnapId, makeRid(_gnapSequenceDatNum, _gnapSequenceId), _gnapId, kSeqSyncWait, 0, 0, 0);
				_gnapSequenceId = 0x110;
				_gnapSequenceDatNum = 0;
				_gameSys->setAnimation(0x110, _gnapId, 0);
				_s41_nextToyVendorSequenceId = 0x111;
				_gameSys->insertSequence(0x111, 1, _s41_currToyVendorSequenceId, 1, kSeqSyncWait, 0, 0, 0);
				_gameSys->setAnimation(_s41_nextToyVendorSequenceId, 1, 3);
				_s41_currToyVendorSequenceId = _s41_nextToyVendorSequenceId;
				_s41_nextToyVendorSequenceId = -1;
				_timers[4] = getRandom(100) + 100;
				_s41_nextKidSequenceId = 0x10F;
				_gameSys->insertSequence(0x10F, 1, _s41_currKidSequenceId, 1, kSeqSyncWait, 0, 0, 0);
				_gameSys->setAnimation(_s41_nextKidSequenceId, 1, 4);
				_s41_currKidSequenceId = _s41_nextKidSequenceId;
				_s41_nextKidSequenceId = -1;
				_timers[5] = getRandom(30) + 20;
				_gnapActionStatus = -1;
			}
			break;
		case kASGiveBackToyUfo:
			if (_gameSys->getAnimationStatus(3) == 2 && _gameSys->getAnimationStatus(4) == 2) {
				_timers[2] = getRandom(30) + 20;
				_timers[3] = getRandom(50) + 200;
				_gameSys->insertSequence(0x124, _gnapId,
					makeRid(_gnapSequenceDatNum, _gnapSequenceId), _gnapId,
					kSeqSyncWait, 0, 0, 0);
				_gnapSequenceId = 0x124;
				_gnapSequenceDatNum = 0;
				_gameSys->setAnimation(0x124, _gnapId, 0);
				_s41_nextToyVendorSequenceId = 0x112;
				_gameSys->insertSequence(0x112, 1, _s41_currToyVendorSequenceId, 1, kSeqSyncWait, 0, 0, 0);
				_gameSys->setAnimation(_s41_nextToyVendorSequenceId, 1, 3);
				_s41_currToyVendorSequenceId = _s41_nextToyVendorSequenceId;
				_s41_nextToyVendorSequenceId = -1;
				_timers[4] = getRandom(100) + 100;
				_s41_nextKidSequenceId = 0x11C;
				_gameSys->insertSequence(0x11C, 1, _s41_currKidSequenceId, 1, kSeqSyncWait, 0, 0, 0);
				_gameSys->setAnimation(_s41_nextKidSequenceId, 1, 4);
				_s41_currKidSequenceId = _s41_nextKidSequenceId;
				_s41_nextKidSequenceId = -1;
				_timers[5] = getRandom(30) + 20;
				_gnapActionStatus = -1;
				clearFlag(kGFGnapControlsToyUFO);
				scene41_updateHotspots();
			}
			break;
		}
	}
	
	if (_gameSys->getAnimationStatus(2) == 2) {
		switch (_toyUfoActionStatus) {
		case kASToyUfoLeaveScene:
			_sceneDone = true;
			break;
		case kASUfoGumAttached:
			_toyUfoNextSequenceId = 0x873;
			_gameSys->insertSequence(0x10873, _toyUfoId, _toyUfoSequenceId | 0x10000, _toyUfoId,
				kSeqSyncWait, 0, _toyUfoX - 365, _toyUfoY - 128);
			_toyUfoSequenceId = _toyUfoNextSequenceId;
			_gameSys->setAnimation(_toyUfoNextSequenceId | 0x10000, _toyUfoId, 2);
			toyUfoSetStatus(kGFJointTaken);
			break;
		default:
			_toyUfoNextSequenceId = toyUfoGetSequenceId();
			_gameSys->insertSequence(_toyUfoNextSequenceId | 0x10000, _toyUfoId + 1, _toyUfoSequenceId | 0x10000, _toyUfoId,
				kSeqSyncWait, 0, _toyUfoX - 274, _toyUfoY - 128);
			_toyUfoSequenceId = _toyUfoNextSequenceId;
			++_toyUfoId;
			_gameSys->setAnimation(_toyUfoNextSequenceId | 0x10000, _toyUfoId, 2);
			break;
		}
		_toyUfoActionStatus = -1;
	}
	
	if (_gameSys->getAnimationStatus(3) == 2 && _s41_nextToyVendorSequenceId != -1) {
		_gameSys->insertSequence(_s41_nextToyVendorSequenceId, 1, _s41_currToyVendorSequenceId, 1, kSeqSyncWait, 0, 0, 0);
		_gameSys->setAnimation(_s41_nextToyVendorSequenceId, 1, 3);
		_s41_currToyVendorSequenceId = _s41_nextToyVendorSequenceId;
		_s41_nextToyVendorSequenceId = -1;
		_timers[4] = getRandom(100) + 100;
	}
	
	if (_gameSys->getAnimationStatus(4) == 2 && _s41_nextKidSequenceId != -1) {
		_gameSys->insertSequence(_s41_nextKidSequenceId, 1, _s41_currKidSequenceId, 1, kSeqSyncWait, 0, 0, 0);
		_gameSys->setAnimation(_s41_nextKidSequenceId, 1, 4);
		_s41_currKidSequenceId = _s41_nextKidSequenceId;
		_s41_nextKidSequenceId = -1;
		_timers[5] = getRandom(30) + 20;
		if (_s41_currKidSequenceId == 0x11E) {
			_toyUfoActionStatus = kASToyUfoRefresh;
			toyUfoFlyTo(getRandom(300) + 500, getRandom(225) + 75, 0, 799, 0, 300, 2);
		}
	}
  
}

} // End of namespace Gnap
