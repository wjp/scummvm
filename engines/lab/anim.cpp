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

/*
 * This code is based on Labyrinth of Time code with assistance of
 *
 * Copyright (c) 1993 Terra Nova Development
 * Copyright (c) 2004 The Wyrmkeep Entertainment Co.
 *
 */

#include "common/endian.h"
#include "lab/lab.h"

namespace Lab {

extern byte **startoffile;

Anim::Anim(LabEngine *vm) : _vm(vm) {
	_header = 0;
	_curBit = 0;
	_numChunks = 1;
	_isBM = false;
	_headerdata._width = 0;
	_headerdata._height = 0;
	_headerdata._fps = 0;
	_headerdata._flags = 0;
	_waitSec = 0;
	_waitMicros = 0;
	_delayMicros = 0;
	_continuous = false;
	_isPlaying = false;
	_isAnim = false;
	_isPal = false;
	_noPalChange = false;
	_donePal = false;
	_frameNum = 0;
	_playOnce = false;
	_buffer = nullptr;
	_diffFile = nullptr;
	_size = 0;
	_rawDiffBM._bytesPerRow = 0;
	_rawDiffBM._flags = 0;
	for (int i = 0; i < 16; i++)
		_rawDiffBM._planes[i] = nullptr;
	_rawDiffBM._rows = 0;
	_waitForEffect = false;
	_stopPlayingEnd = false;
	_sampleSpeed = 0;
	_doBlack = false;
	_diffWidth = 0;
	_diffHeight = 0;
	_dataBytesPerRow = 0;
	DrawBitMap = &_vm->_graphics->_dispBitMap;

	for (int i = 0; i < 3 * 256; i++)
		_diffPalette[i] = 0;
}

/*------------------------ unDiff Horizontal Memory -------------------------*/

/*****************************************************************************/
/* Undiffs a piece of memory when header size is a byte, and copy/skip size  */
/* is also a byte.                                                           */
/*****************************************************************************/
void Anim::unDiffByteByte(byte *dest, byte *diff) {
	uint16 skip, copy;

	while (1) {
		skip = *diff;
		diff++;
		copy = *diff;
		diff++;

		if (skip == 255) {
			if (copy == 0) {
				skip = READ_LE_UINT16(diff);
				diff += 2;
				copy = READ_LE_UINT16(diff);
				diff += 2;
			} else if (copy == 255)
				return;
		}

		dest += skip;
		memcpy(dest, diff, copy);
		dest += copy;
		diff += copy;
	}
}

/*****************************************************************************/
/* Undiffs a piece of memory when header size is a byte, and copy/skip size  */
/* is a word.                                                                */
/*****************************************************************************/
void Anim::unDiffByteWord(uint16 *dest, uint16 *diff) {
	uint16 skip, copy;

	while (1) {
		skip = ((byte *)diff)[0];
		copy = ((byte *)diff)[1];

		diff++;

		if (skip == 255) {
			if (copy == 0) {
				skip = READ_LE_UINT16(diff);
				diff++;
				copy = READ_LE_UINT16(diff);
				diff++;
			} else if (copy == 255)
				return;
		}

		dest += skip;

		while (copy > 3) {
			*dest = READ_LE_UINT16(diff);
			dest++;
			diff++;

			*dest = READ_LE_UINT16(diff);
			dest++;
			diff++;

			*dest = READ_LE_UINT16(diff);
			dest++;
			diff++;

			*dest = READ_LE_UINT16(diff);
			dest++;
			diff++;

			copy -= 4;
		}

		while (copy) {
			*dest = READ_LE_UINT16(diff);
			dest++;
			diff++;
			copy--;
		}
	}
}

/*------------------------- unDiff Vertical Memory --------------------------*/

/*****************************************************************************/
/* Undiffs a piece of memory when header size is a byte, and copy/skip size  */
/* is a byte.                                                                */
/*****************************************************************************/
void Anim::VUnDiffByteByte(byte *dest, byte *diff, uint16 bytesPerRow) {
	byte *curPtr;
	uint16 skip, copy;
	uint16 counter = 0;


	while (counter < _dataBytesPerRow) {
		curPtr = dest + counter;

		for (;;) {
			skip = *diff;
			diff++;
			copy = *diff;
			diff++;

			if (skip == 255) {
				counter += copy;
				break;
			}

			else {
				curPtr += (skip * bytesPerRow);

				while (copy) {
					copy--;
					*curPtr = *diff;
					curPtr += bytesPerRow;
					diff++;
				}
			}
		}
	}
}

/*****************************************************************************/
/* Undiffs a piece of memory when header size is a byte, and copy/skip size  */
/* is a word.                                                                */
/*****************************************************************************/
void Anim::VUnDiffByteWord(uint16 *dest, uint16 *diff, uint16 bytesPerRow) {
	uint16 *curPtr;
	uint16 skip, copy;
	uint16 counter = 0;

	uint16 wordsPerRow = bytesPerRow / 2;

	while (counter < (_dataBytesPerRow >> 1)) {
		curPtr = dest + counter;

		for (;;) {
			skip = ((byte *)diff)[0];
			copy = ((byte *)diff)[1];

			diff++;


			if (skip == 255) {
				counter += copy;
				break;
			}

			else {
				curPtr += (skip * wordsPerRow);

				while (copy) {
					*curPtr = *diff; //swapUShort(*diff);
					curPtr += wordsPerRow;
					diff++;
					copy--;
				}
			}
		}
	}
}

/*****************************************************************************/
/* Undiffs a piece of memory when header size is a byte, and copy/skip size  */
/* is a long.                                                                */
/*****************************************************************************/
void Anim::VUnDiffByteLong(uint32 *dest, uint32 *diff, uint16 bytesPerRow) {
	uint32 *_curPtr;
	uint16 skip, copy;

	uint16 counter = 0;
	byte *diff1 = (byte *)diff;

	uint16 longsperrow = bytesPerRow / 4;

	while (counter < (_dataBytesPerRow >> 2)) {
		_curPtr = dest + counter;

		for (;;) {
			skip = *diff1;
			diff1++;

			copy = *diff1;
			diff1++;


			if (skip == 255) {
				counter += copy;
				break;
			}

			else {
				_curPtr += (skip * longsperrow);

				while (copy) {
					*_curPtr = *(uint32 *)diff1; //swapULong(*diff);
					_curPtr += longsperrow;
					diff1 += 4;
					copy--;
				}
			}
		}
	}
}

/*****************************************************************************/
/* Runlength decodes a chunk of memory.                                      */
/*****************************************************************************/
void Anim::runLengthDecode(byte *dest, byte *source) {
	int8 num;
	int16 count;

	while (1) {
		num = (int8)*source;
		source++;

		if (num == 127) {
			return;
		} else if (num > '\0') {
			memcpy(dest, source, num);
			source += num;
			dest   += num;
		} else {
			count = (int16)(-num);
			num   = *source;
			source++;

			while (count) {
				*dest = num;
				dest++;
				count--;
			}
		}
	}
}

/*****************************************************************************/
/* Does a vertical run length decode.                                        */
/*****************************************************************************/
void Anim::VRunLengthDecode(byte *dest, byte *source, uint16 bytesPerRow) {
	int8 num;
	int16 count;
	byte *top = dest;

	for (uint16 i = 0; i < _dataBytesPerRow; i++) {
		dest = top;
		dest += i;

		num = (int8)*source;
		source++;

		while (num != 127) {
			if (num > '\0') {
				while (num) {
					*dest = *source;
					source++;
					dest += bytesPerRow;
					num--;
				}
			} else {
				count = (int16)(-num);
				num   = (int8)*source;
				source++;

				while (count) {
					*dest = num;
					dest += bytesPerRow;
					count--;
				}
			}

			num = *source;
			source++;
		}
	}
}

/*****************************************************************************/
/* Does the undiffing between the bitmaps.                                   */
/*****************************************************************************/
void Anim::unDiff(byte *newBuf, byte *oldBuf, byte *diffData, uint16 bytesPerRow, bool isV) {
	diffData++;
	byte bufType = *diffData;
	diffData++;

	if (isV) {
		if (bufType == 0)
			VUnDiffByteByte(newBuf, diffData, bytesPerRow);
		else if (bufType == 1)
			VUnDiffByteWord((uint16 *)newBuf, (uint16 *)diffData, bytesPerRow);
		else if (bufType == 3)
			VUnDiffByteLong((uint32 *)newBuf, (uint32 *)diffData, bytesPerRow);
	} else {
		if (bufType == 0)
			unDiffByteByte(newBuf, diffData);
		else if (bufType == 1)
			unDiffByteWord((uint16 *)newBuf, (uint16 *)diffData);
	}
}

void Anim::readBlock(void *Buffer, uint32 Size, byte **File) {
	memcpy(Buffer, *File, (size_t)Size);
	(*File) += Size;
}

void Anim::diffNextFrame() {
	if (_header == 65535)  /* Already done. */
		return;

	if (_vm->_graphics->_dispBitMap._flags & BITMAPF_VIDEO) {
		_vm->_graphics->_dispBitMap._planes[0] = _vm->_graphics->getCurrentDrawingBuffer();
		_vm->_graphics->_dispBitMap._planes[1] = _vm->_graphics->_dispBitMap._planes[0] + 0x10000;
		_vm->_graphics->_dispBitMap._planes[2] = _vm->_graphics->_dispBitMap._planes[1] + 0x10000;
		_vm->_graphics->_dispBitMap._planes[3] = _vm->_graphics->_dispBitMap._planes[2] + 0x10000;
		_vm->_graphics->_dispBitMap._planes[4] = _vm->_graphics->_dispBitMap._planes[3] + 0x10000;
	}

	_vm->_event->mouseHide();

	while (1) {
		if (_curBit >= _numChunks) {
			_vm->_event->mouseShow();

			if (!_isBM) {
				if (_headerdata._fps) {
					_vm->waitForTime(_waitSec, _waitMicros);
					_vm->addCurTime(0L, _delayMicros, &_waitSec, &_waitMicros);
				}

				if (_isPal && !_noPalChange) {
					_vm->_graphics->setPalette(_diffPalette, 256);
					_isPal = false;
				}

				_donePal = true;
			}

			if (_isPal && !_noPalChange && !_isBM && !_donePal) {
				_vm->_graphics->setPalette(_diffPalette, 256);
				_isPal = false;
			}

			_donePal = false;

			_frameNum++;

			if ((_frameNum == 1) && (_continuous || (!_playOnce)))
				_buffer = _diffFile;

			_isAnim = (_frameNum >= 3) && (!_playOnce);
			_curBit = 0;

			if (_vm->_graphics->_dispBitMap._flags & BITMAPF_VIDEO)
				_vm->_graphics->screenUpdate();

			return; /* done with the next frame. */
		}

		_vm->_music->updateMusic();
		_header = READ_LE_UINT32(_diffFile);
		_diffFile += 4;

		_size = READ_LE_UINT32(_diffFile);
		_diffFile += 4;

		switch (_header) {
		case 8L:
			readBlock(_diffPalette, _size, &_diffFile);
			_isPal = true;
			break;

		case 10L:
			_rawDiffBM._planes[_curBit] = _diffFile;

			if (_isBM)
				_diffFile += _size;
			else {
				readBlock(DrawBitMap->_planes[_curBit], _size, &_diffFile);
			}

			_curBit++;
			break;

		case 11L:
			_diffFile += 4;
			runLengthDecode(DrawBitMap->_planes[_curBit], _diffFile);
			_curBit++;
			_diffFile += _size - 4;
			break;

		case 12L:
			_diffFile += 4;
			VRunLengthDecode(DrawBitMap->_planes[_curBit], _diffFile, DrawBitMap->_bytesPerRow);
			_curBit++;
			_diffFile += _size - 4;
			break;

		case 20L:
			unDiff(DrawBitMap->_planes[_curBit], _vm->_graphics->_dispBitMap._planes[_curBit], _diffFile, DrawBitMap->_bytesPerRow, false);
			_curBit++;
			_diffFile += _size;
			break;

		case 21L:
			unDiff(DrawBitMap->_planes[_curBit], _vm->_graphics->_dispBitMap._planes[_curBit], _diffFile, DrawBitMap->_bytesPerRow, true);
			_curBit++;
			_diffFile += _size;
			break;

		case 25L:
			_curBit++;
			break;

		case 26L:
			_curBit++;
			break;

		case 30L:
		case 31L:
			if (_waitForEffect) {
				while (_vm->_music->isSoundEffectActive()) {
					_vm->_music->updateMusic();
					_vm->waitTOF();
				}
			}

			_size -= 8L;


			_diffFile += 4;
			_sampleSpeed = READ_LE_UINT16(_diffFile);
			_diffFile += 4;

			_vm->_music->playSoundEffect(_sampleSpeed, _size, _diffFile);
			_diffFile += _size;
			break;
		case 65535L:
			if ((_frameNum == 1) || _playOnce || _stopPlayingEnd) {
				bool didTOF = false;

				if (_waitForEffect) {
					while (_vm->_music->isSoundEffectActive()) {
						_vm->_music->updateMusic();
						_vm->waitTOF();

						if (_vm->_graphics->_dispBitMap._flags & BITMAPF_VIDEO)
							didTOF = true;
					}
				}

				_isPlaying = false;
				_vm->_event->mouseShow();

				if (!didTOF)
					_vm->_graphics->screenUpdate();

				return;
			}

			_frameNum = 4;  /* Random frame number so it never gets back to 2 */
			_diffFile = _buffer;
			break;

		default:
			_diffFile += _size;
			break;
		}
	}
}

/*****************************************************************************/
/* A separate task launched by readDiff.  Plays the DIFF.                    */
/*****************************************************************************/
void Anim::playDiff(byte *buffer) {
	_waitSec   = 0L;
	_waitMicros = 0L;
	_delayMicros = 0L;
	_header      = 0;
	_curBit = 0;
	_frameNum = 0;
	_numChunks   = 1;
	_donePal     = false;
	_stopPlayingEnd = false;

	_isPlaying   = true;

	if (_doBlack) {
		_doBlack = false;
		_vm->_graphics->blackScreen();
	}

	_diffFile = buffer;

	_continuous = false;
	uint32 signature = READ_BE_UINT32(_diffFile);
	_diffFile += 4;

	_header = READ_LE_UINT32(_diffFile);
	_diffFile += 4;

	if ((signature != MKTAG('D', 'I', 'F', 'F')) || (_header != 1219009121L)) {
		_isPlaying = false;
		return;
	}

	_header = READ_LE_UINT32(_diffFile);
	_diffFile += 4;

	_size = READ_LE_UINT32(_diffFile);
	_diffFile += 4;

	if (_header == 0) {
		// sizeof(headerdata) != 18, but the padding might be at the end
		_headerdata._version = READ_LE_UINT16(_diffFile);
		_diffFile += 2;
		_headerdata._width = READ_LE_UINT16(_diffFile);
		_diffFile += 2;
		_headerdata._height = READ_LE_UINT16(_diffFile);
		_diffFile += 2;
		_headerdata._depth = _diffFile[0];
		_diffFile++;
		_headerdata._fps = _diffFile[0];
		_diffFile++;
		_headerdata._bufferSize = READ_LE_UINT32(_diffFile);
		_diffFile += 4;
		_headerdata._machine = READ_LE_UINT16(_diffFile);
		_diffFile += 2;
		_headerdata._flags = READ_LE_UINT32(_diffFile);
		_diffFile += 4;

		_diffFile += _size - 18;

		_continuous = CONTINUOUS & _headerdata._flags;
		_diffWidth = _headerdata._width;
		_diffHeight = _headerdata._height;
		_dataBytesPerRow = _diffWidth;

		_numChunks = (((int32) _diffWidth) * _diffHeight) / 0x10000;

		if ((uint32)(_numChunks * 0x10000) < (uint32)(((int32) _diffWidth) * _diffHeight))
			_numChunks++;
	} else {
		return;
	}

	for (_header = 0; _header < 8; _header++)
		_rawDiffBM._planes[_header] = NULL;

	if (_headerdata._fps)
		_delayMicros = ONESECOND / _headerdata._fps;

	if (_playOnce) {
		while (_header != 65535)
			diffNextFrame();
	} else
		diffNextFrame();
}

/*****************************************************************************/
/* Stops an animation from running.                                          */
/*****************************************************************************/
void Anim::stopDiff() {
	if (_isPlaying && _isAnim)
		_vm->_graphics->blackScreen();
}

/*****************************************************************************/
/* Stops an animation from running.                                          */
/*****************************************************************************/
void Anim::stopDiffEnd() {
	if (_isPlaying) {
		_stopPlayingEnd = true;
		while (_isPlaying) {
			_vm->_music->updateMusic();
			diffNextFrame();
		}
	}
}

/*****************************************************************************/
/* Reads in a DIFF file.                                                     */
/*****************************************************************************/
bool Anim::readDiff(byte *buffer, bool playOnce) {
	_playOnce = playOnce;
	playDiff(buffer);
	return true;
}

} // End of namespace Lab
