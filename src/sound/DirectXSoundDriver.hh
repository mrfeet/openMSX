// $Id$

#ifndef DIRECTXSOUNDDRIVER_HH
#define DIRECTXSOUNDDRIVER_HH
#ifdef _WIN32

#include "SoundDriver.hh"
#include "Schedulable.hh"
#include "SettingListener.hh"
#define DIRECTSOUND_VERSION 0x0500
#include <windows.h>
#include <dsound.h>

namespace openmsx {

class Scheduler;
class GlobalSettings;
class Mixer;
class IntegerSetting;
class BooleanSetting;

class DirectXSoundDriver : public SoundDriver, private Schedulable,
                           private SettingListener
{
public:
	DirectXSoundDriver(Scheduler& scheduler, GlobalSettings& globalSettings,
	                   Mixer& mixer, unsigned sampleRate, unsigned bufferSize);
	virtual ~DirectXSoundDriver();

	virtual void lock();
	virtual void unlock();

	virtual void mute();
	virtual void unmute();

	virtual unsigned getFrequency() const;
	virtual unsigned getSamples() const;

	virtual void updateStream(const EmuTime& time);

private:
	void dxClear();
	int dxCanWrite(unsigned start, unsigned size);
	void dxWriteOne(short* buffer, unsigned lockSize);
	void dxWrite(short* buffer, unsigned count);
	void reInit();

	// Schedulable
	void executeUntil(const EmuTime& time, int userData);
	const std::string& schedName() const;

	// SettingListener
	virtual void update(const Setting* setting);

	enum DxState { DX_SOUND_DISABLED, DX_SOUND_ENABLED, DX_SOUND_RUNNING };
	DxState state;
	unsigned bufferOffset;
	unsigned bufferSize;
	unsigned fragmentSize;
	int skipCount;
	LPDIRECTSOUNDBUFFER primaryBuffer;
	LPDIRECTSOUNDBUFFER secondaryBuffer;
	LPDIRECTSOUND directSound;

	Mixer& mixer;
	unsigned frequency;

	short* mixBuffer;
	EmuTime prevTime;
	EmuDuration interval1;
	//EmuDuration intervalAverage;

	IntegerSetting& speedSetting;
	BooleanSetting& throttleSetting;
};

} // namespace openmsx

#endif // _WIN32
#endif // DIRECTXSOUNDDRIVER_HH
