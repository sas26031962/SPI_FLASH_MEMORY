#ifndef DEFINE
#define DEFINE
//-----------------------------------------------------------------------
// КОНСТАНТЫ
//-----------------------------------------------------------------------

const int SSPin = 10;   //CS-pin
const byte WREN = 0x06; //Команда разрешения записи
const byte WRDI = 0x04; //Команда запрета записи
const byte READ = 0x03; //Команда чтения массива
const byte PP = 0x02;   //Команда записи байта (страницы)

const byte BLOCK_ERASE_4KB = 0x20;    //Команда BlockErase (4kB)
const byte BLOCK_ERASE_32KB = 0x52;   //Команда BlockErase (32kB)
const byte BLOCK_ERASE_64KB = 0xd8;   //Команда BlockErase (64kB)

const byte CHIP_ERASE1 = 0x60;        //Команда ChipErase1 
const byte CHIP_ERASE2 = 0xC7;        //Команда ChipErase2 

const byte SET_SECTOR_PROTECTION = 0x36;   //Команда установки защиты с сектора
const byte CLEAR_SECTOR_PROTECTION = 0x39;   //Команда снятия защиты с сектора

const byte RSTATUS = 0x05;   //Команда чтения регистра статуса
const byte WSTATUS1 = 0x01;  //Команда записи байта 1 регистра статуса
const byte WSTATUS2 = 0x31;  //Команда записи байта 2 регистра статуса

#endif
