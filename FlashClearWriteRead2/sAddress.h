#ifndef S_ADDRESS
#define S_ADDRESS
//=======================================================================
// Структура для адресации микросхемы FLASH памяти
//=======================================================================
struct sAddress{
  byte First;     //Старший байт
  byte Second;    //Средний байт
  byte Third;     //Младший байт
  void view();    //Просмотр значения адреса через Serial
  void get(int x);//Извлечение целевого адреса
};

//Просмотр значения адреса через Serial
void sAddress::view()
{
  Serial.println("Current FLASH address: " + 
    String(this->First, HEX) + 
    String(this->Second, HEX) + 
    String(this->Third, HEX)
    );
}
//Извлечение целевого адреса
void sAddress::get(int x)
{
    this->First = ((x >> 16) & 0xff); 
    this->Second = ((x >> 8) & 0xff); 
    this->Third = (x & 0xff);
    
}

#endif
