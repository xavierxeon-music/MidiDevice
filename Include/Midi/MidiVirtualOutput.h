#ifndef MidiVirtualOutputH
#define MidiVirtualOutputH

#include "../../Private/MidiRtMidiOutput.h"

namespace Midi
{
   namespace Virtual
   {
      // creates a virtual midi output
      class Output : public RtMidi::Output
      {
      public:
         Output(const QString& portName);
         virtual ~Output();

      public:
         void open() override;
         void close() override;
         bool isOpen() const override;

      private:
         bool isOpened;
      };
   } // namespace Virtual
} // namespace Midi

#endif // NOT MidiVirtualOutputH
