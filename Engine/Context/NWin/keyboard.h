#pragma once
#include <unordered_map>
#include "timer.h"

#include "keys.h"


namespace NWin {


enum KeyEventEnum{
	NWIN_KeyEventNONE= 0,
	NWIN_KeyPressed  = 1,
	NWIN_KeyReleased = 2
};

struct KeyEvent {
	Key          key;
	KeyEventEnum eventType;
};

struct KeyMasterData {
	KeyEventEnum _lastEvent; //Record updates _last event and places _last into _prev
    KeyEventEnum _prevEvent;
	timeMl       lastEventTime       = 0;
	timeMl       hasBeenPressedFor   = 0;
	timeMl       _cooldownTimeBuffer = 0;
	timeMl       _cooldownTime		 = 0;
};

typedef KeyEvent* KeyEventQueue;

class Keyboard {
private: 
	    KeyEventQueue _eventsQueue; //Array of keys
		std::unordered_map<Key, KeyMasterData> keyMaster;
		int _length = 0;
		int _index  = 0;
		Timer _timer;

public:
		void  create(int bufLength);
		void  destroy();
		void  resizeEventQueue(int newLength);
		bool  record(const KeyEvent& event, bool failIfFull = 0);
		const KeyEventQueue getEventQueue();
		bool  isEventInQueue(const KeyEvent& event);

		bool onKeyPress(Key  key);
		bool onKeyRelease(Key key);

        bool isKeyPressed(Key key);

		void setKeyCooldown(Key key	,timeMl t);
		const KeyMasterData* getKeyData(Key key);
		timeMl getKeyPressDuration(Key key);

		void update();	
};
}
