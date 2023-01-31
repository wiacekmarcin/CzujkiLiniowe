#ifndef CRC8_H
#define CRC8_H
#define CRC8_DEFAULT_POLYNOME       0x07

#include <stdint.h>

class CRC8
{
public:
  CRC8();

  // set parameters to default
  void     reset();       // set all to constructor defaults
  void     restart();     // reset crc with same parameters.

  // set parameters
  inline void     setPolynome(uint8_t polynome)  { _polynome = polynome; };
  inline void     setStartXOR(uint8_t start)     { _startMask = start; };
  inline void     setEndXOR(uint8_t end)         { _endMask = end; };
  inline void     setReverseIn(bool reverseIn)   { _reverseIn = reverseIn; };
  inline void     setReverseOut(bool reverseOut) { _reverseOut = reverseOut; };

  void     add(uint8_t value);
  void     add(uint8_t * array, uint32_t length);

  uint8_t  getCRC();  // returns CRC
  uint32_t count()    { return _count; };

private:
  uint8_t  _reverse(uint8_t value);
  void     _update(uint8_t value);

  uint8_t  _polynome;
  uint8_t  _startMask;
  uint8_t  _endMask;
  uint8_t  _crc;
  bool     _reverseIn;
  bool     _reverseOut;
  bool     _started;
  uint32_t _count;
};


#endif // CRC8_H
