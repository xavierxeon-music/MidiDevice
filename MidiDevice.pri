macos{
   DEFINES += __MACOSX_CORE__
   LIBS += \
     -framework CoreMIDI \
     -framework CoreAudio \
     -framework CoreFoundation
}

win32{
   DEFINES += __WINDOWS_MM__
   LIBS += Winmm.lib
}

linux {
   DEFINES += __UNIX_JACK__
   LIBS += -ljack
}

INCLUDEPATH += $$PWD/Include

HEADERS += \
   $$PWD/Include/Midi/MidiPhysicalInput.h \
   $$PWD/Include/Midi/MidiPhysicalOutput.h \
   $$PWD/Include/Midi/MidiVirtualInput.h \
   $$PWD/Include/Midi/MidiVirtualOutput.h \
   $$PWD/Private/MidiRtMidiBase.h \
   $$PWD/Private/MidiRtMidiInput.h \
   $$PWD/Private/MidiRtMidiOutput.h \
   $$PWD/Private/MidiToolBridge.hpp \
   $$PWD/Private/RtMidi4.h

SOURCES += \
   $$PWD/MidiPhysicalInput.cpp \
   $$PWD/MidiPhysicalOutput.cpp \
   $$PWD/MidiRtMidiBase.cpp \
   $$PWD/MidiRtMidiInput.cpp \
   $$PWD/MidiRtMidiOutput.cpp \
   $$PWD/MidiVirtualInput.cpp \
   $$PWD/MidiVirtualOutput.cpp \
   $$PWD/Private/RtMidi4.cpp
