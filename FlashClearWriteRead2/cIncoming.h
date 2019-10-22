/*******************************************************************
 * Строка с командой начинается  символом "решётки", а оканчивается 
 * символом ASCII(10). В начале строки могут шумовые символы.
 * Тело строки с командой содержит код команды и параметр, которые
 * разделяются символом "двоеточие".
 * Команды:
 * > Чтение показаний аналогового канала
 * get:<номер канала>
 * > Чтение статуса памяти
 * status
 * > Чтение страницы памяти
 * readPage:<address>
 * > Запись страницы памяти
 * writePage:<address>
 * > Стирание страницы памяти
 * erasePage:<address>
 * > Чтение значения аналогового сигнала (номера каналов от 1 до 12)
 * get:<chennel>
 *******************************************************************/
#ifndef C_INCOMING
#define C_INCOMING
//-----------------------------------------------------------------------
// КОНСТАНТЫ
//-----------------------------------------------------------------------

#define LF 10
#define OCTOTHORP 0x23

//-----------------------------------------------------------------------
// ПОДКЛЮЧАЕМЫЕ ФАЙЛЫ
//-----------------------------------------------------------------------
#include "cFlash.h"
#include "sGlobal.h"

//-----------------------------------------------------------------------
// КЛАССЫ И СТРУКТУРЫ
//-----------------------------------------------------------------------
enum IncomingPhase{
    NONE = 0,
    SEARCH_HEAD = 1,
    SEARCH_TAIL = 2,
    SEARCH_COMPLETE = 3
};

class cIncoming
{
protected:
String Command;
String Parameter;

public:
String Buffer;
char incomingByte;
char Phase;
int InvalidBytes;

cIncoming();
void operate();
void execute();  

};//End of class cIncoming

cIncoming::cIncoming()
{
  this->Buffer = "";
  this->incomingByte = 0;
  this->InvalidBytes = 0;
  this->Command = "";
  this->Parameter = "";
  this->Phase = SEARCH_HEAD;
}

// Реализация приёма команды
void cIncoming::operate()
{
  if(Serial.available() > 0) 
  {
    //Чтение байта из последовательного порта
    this->incomingByte = Serial.read();
    //Разбор принятого байта в зависимости от фазы приёма
    switch(this->Phase)
    {
      case SEARCH_HEAD:
      if(this->incomingByte == OCTOTHORP)
      {
        this->Phase = SEARCH_TAIL;  
        //Serial.print("#");
      }
      else
      {
        this->InvalidBytes++;
      }
      break;

      case SEARCH_TAIL:
      if(this->incomingByte == LF)
      {
        this->Phase = SEARCH_COMPLETE;  
        //Serial.print("!");
      }
      else
      {
        this->Buffer += String(this->incomingByte);
        //Serial.print(".");
      }
      break;

      case SEARCH_COMPLETE:

      break;

      case NONE:

      break;

      default:
      break;
    }//End of switch(this->Phase)
    
  }//End of while(Serial.available() > 0)
   
}//End of void cIncoming::operate()

// Реализация функциональности
void cIncoming::execute()
{
  if(this->Phase == SEARCH_COMPLETE)
  {
    Serial.println("String:" + this->Buffer);
    int pos = this->Buffer.indexOf(':');
    if( pos >= 0 )
    {
      this->Command = this->Buffer.substring(0,pos);
      this->Parameter = this->Buffer.substring(pos+1);
    }
    else
    {
      this->Command = this->Buffer;
      this->Parameter = "";
    }

    //Индикация результатов разбора
    //Serial.print("Command:");
    //Serial.print(this->Command);
    //Serial.print(" Parameter:");
    //Serial.println(this->Parameter);

    if(Command == "get")
    {
      int x = this->Parameter.toInt();
      int y = 0xff;
      
      //Serial.print("Command=get");
      //Serial.print(" Parameter=");
      //Serial.println(x);
      switch(x)
      {
        case 0:// Ошибочное значение
          Serial.println("Format error");
        break;
        
        case 1:
          y = analogRead(0);
          Serial.print("Chennel01:");
          Serial.println(y);
        break;
        
        case 2:
          y = analogRead(1);
          Serial.print("Chennel02:");
          Serial.println(y);
        break;
        
        case 3:
          y = analogRead(2);
          Serial.print("Chennel03:");
          Serial.println(y);
        break;
        
        case 4:
         y = analogRead(3);
         Serial.print("Chennel04:");
         Serial.println(y);
        break;
        
        case 5:
          y = analogRead(4);
          Serial.print("Chennel05:");
          Serial.println(y);
        break;
        
        case 6:
          y = analogRead(5);
          Serial.print("Chennel06:");
          Serial.println(y);
        break;
        
        case 7:
          y = analogRead(6);
          Serial.print("Chennel07:");
          Serial.println(y);
        break;
        
        case 8:
          y = analogRead(7);
          Serial.print("Chennel08:");
          Serial.println(y);
        break;
        
        case 9:
          y = analogRead(8);
          Serial.print("Chennel09:");
          Serial.println(y);
        break;
        
        case 10:
          y = analogRead(9);
          Serial.print("Chennel10:");
          Serial.println(y);
        break;
        
        case 11:
          y = analogRead(10);
          Serial.print("Chennel11:");
          Serial.println(y);
        break;
        
        case 12:
          y = analogRead(11);
          Serial.print("Chennel12:");
          Serial.println(y);
        break;

        default:
          Serial.println("Chennel number error");
        break;
      }
    }//End of if(Command == "get")
    //------------------------------------------------------
    if(Command == "status")
    {
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
      //Serial.println("Command=status");
      
    }//End of Command == "status"
    //------------------------------------------------------
    if(Command == "readPage")
    {
      Serial.print("Command=readPage:");
      int x = this->Parameter.toInt();
      sGlobal::Flash.Address.get(x);
      sGlobal::Flash.Address.view();

     sGlobal::Flash.Timer.start();
      sGlobal::Flash.readPage();
      sGlobal::Flash.Timer.stop();
      sGlobal::Flash.Timer.view();

      sGlobal::Flash.view();
      
    }//End of Command == "readPage"
    
    //------------------------------------------------------
    if(Command == "writePage")
    {
       // Заполнение массива данных в записи в ПЗУ!!!!
      Serial.println("Prepare data data in the buffer");
      for(int i = 0; i <256; i++) sGlobal::Flash.data[i] = i;

     Serial.print("Command=writePage:");
      int x = this->Parameter.toInt();
      sGlobal::Flash.Address.get(x);
      sGlobal::Flash.Address.view();

      // Разблокирование сектора для записи
  
      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Проверка бита разрешения записи
      if(sGlobal::Flash.getWriteEnable())
        Serial.println("WEL bit is true");
      else
        Serial.println("WEL bit is false");
        
      // Разрешение сектора 0 для записи
      Serial.println("Clear sector protection from this address"); 
 
      sGlobal::Flash.clearSectorProtection();

      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      Serial.println("Write data from array");
      sGlobal::Flash.Timer.start();
      sGlobal::Flash.writePage();
      sGlobal::Flash.Timer.stop();
      sGlobal::Flash.Timer.view();
  
    }//End of Command == "writePage"
    
    //------------------------------------------------------
    if(Command == "eraseBlock4K")
    {
      Serial.print("Command=eraseBlock4K:");
      int x = this->Parameter.toInt();
      sGlobal::Flash.Address.get(x);
      sGlobal::Flash.Address.view();

      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      // Разрешение сектора 0 для записи
      Serial.println("Enable sector for erasing");
      sGlobal::Flash.clearSectorProtection();

      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
 
      // Очистка блока:
  
      Serial.println("Erase block 4kB using 20h opcode");
      sGlobal::Flash.Timer.start();
      sGlobal::Flash.eraseBlock4K();
      
      sGlobal::Flash.Status.get();
      while(sGlobal::Flash.Status.First & 0x1)
      {
        sGlobal::Flash.Status.get();
      }
      sGlobal::Flash.Timer.stop();
      sGlobal::Flash.Timer.view();
      Serial.println("Block 4kB from SPI flash is clear!");

      // Выставление запрета записи:
      Serial.println("Clear WEL bit");
      sGlobal::Flash.clearWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
  
    }//End of Command == "eraseBlock4K"
    
    //------------------------------------------------------
    if(Command == "eraseBlock32K")
    {
      Serial.print("Command=eraseBlock32K:");
      int x = this->Parameter.toInt();
      sGlobal::Flash.Address.get(x);
      sGlobal::Flash.Address.view();

      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      // Разрешение сектора 0 для записи
      Serial.println("Enable sector for erasing");
      sGlobal::Flash.clearSectorProtection();

      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
 
      // Очистка блока:
  
      Serial.println("Erase block 32kB using 52h opcode");
      sGlobal::Flash.Timer.start();
      sGlobal::Flash.eraseBlock32K();
      
      sGlobal::Flash.Status.get();
      while(sGlobal::Flash.Status.First & 0x1)
      {
        sGlobal::Flash.Status.get();
      }
      sGlobal::Flash.Timer.stop();
      sGlobal::Flash.Timer.view();
      Serial.println("Block 32kB from SPI flash is clear!");

      // Выставление запрета записи:
      Serial.println("Clear WEL bit");
      sGlobal::Flash.clearWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
  
    }//End of Command == "eraseBlock32K"
    
    //------------------------------------------------------
    if(Command == "eraseBlock64K")
    {
      Serial.print("Command=eraseBlock64K:");
      int x = this->Parameter.toInt();
      sGlobal::Flash.Address.get(x);
      sGlobal::Flash.Address.view();

      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      // Разрешение сектора 0 для записи
      Serial.println("Enable sector for erasing");
      sGlobal::Flash.clearSectorProtection();

      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
 
      // Очистка блока:
  
      Serial.println("Erase block 64kB using 0D8h opcode");
      sGlobal::Flash.Timer.start();
      sGlobal::Flash.eraseBlock64K();
      
      sGlobal::Flash.Status.get();
      while(sGlobal::Flash.Status.First & 0x1)
      {
        sGlobal::Flash.Status.get();
      }
      sGlobal::Flash.Timer.stop();
      sGlobal::Flash.Timer.view();
      Serial.println("Block 64kB from SPI flash is clear!");

      // Выставление запрета записи:
      Serial.println("Clear WEL bit");
      sGlobal::Flash.clearWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
  
    }//End of Command == "eraseBlock64K"
    
    //------------------------------------------------------
    if(Command == "eraseChip1")
    {
      Serial.print("Command=eraseChip1:");
      //int x = this->Parameter.toInt();
      //sGlobal::Flash.Address.get(x);
      //sGlobal::Flash.Address.view();
      Serial.println();

      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      // Разрешение сектора 0 для записи
      Serial.println("Enable sector for erasing");
      sGlobal::Flash.clearSectorProtection();

      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
 
      // Очистка кристалла:
  
      Serial.println("Erase chip using 60h opcode");
      sGlobal::Flash.Timer.start();
      sGlobal::Flash.eraseChip1();
      
      sGlobal::Flash.Status.get();
      while(sGlobal::Flash.Status.First & 0x1)
      {
        sGlobal::Flash.Status.get();
      }
      sGlobal::Flash.Timer.stop();
      sGlobal::Flash.Timer.view();
      Serial.println("Chip SPI flash is clear!");

      // Выставление запрета записи:
      Serial.println("Clear WEL bit");
      sGlobal::Flash.clearWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
  
    }//End of Command == "eraseChip1"
    
    //------------------------------------------------------
    if(Command == "eraseChip2")
    {
      Serial.print("Command=eraseChip2:");
      //int x = this->Parameter.toInt();
      //sGlobal::Flash.Address.get(x);
      //sGlobal::Flash.Address.view();
      Serial.println();

      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      // Разрешение сектора 0 для записи
      Serial.println("Enable sector for erasing");
      sGlobal::Flash.clearSectorProtection();

      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();

      // Установка бита разрешения записи
      Serial.println("Set WEL bit");
      sGlobal::Flash.setWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
 
      // Очистка кристалла:
  
      Serial.println("Erase chip using 0C7h opcode");
      sGlobal::Flash.Timer.start();
      sGlobal::Flash.eraseChip1();
      
      sGlobal::Flash.Status.get();
      while(sGlobal::Flash.Status.First & 0x1)
      {
        sGlobal::Flash.Status.get();
      }
      sGlobal::Flash.Timer.stop();
      sGlobal::Flash.Timer.view();
      Serial.println("Chip SPI flash is clear!");

      // Выставление запрета записи:
      Serial.println("Clear WEL bit");
      sGlobal::Flash.clearWriteEnable();
      // Чтение регистра статуса
      sGlobal::Flash.Status.get();
      sGlobal::Flash.Status.view();
  
    }//End of Command == "eraseChip1"
    
    
    //Возврат к поиску начального маркера
    this->Buffer = "";
    this->Phase = SEARCH_HEAD;
  }
}//End of void cIncoming::execute()

#endif
