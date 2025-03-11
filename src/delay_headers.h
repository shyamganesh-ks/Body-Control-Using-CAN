void delay_sec(unsigned int seconds)
{
	T0PR  = 15000000 - 1;
	T0TCR = 0X01;
	while(T0TC < seconds);
	T0TCR = 0X03;
	T0TCR = 0X00;
}
void delay_ms(unsigned int ms) //ms -> milliseconds
{
		T0PR  = 15000 - 1;
		T0TCR = 0X01;
   	while(T0TC < ms);
  	T0TCR = 0X03;
   	T0TCR = 0X00;
}
void delay_us(unsigned int us) //us -> microseconds
{
    T0PR  = 15 - 1;
    T0TCR = 0X01;
    while(T0TC < us);
    T0TCR = 0X03;
    T0TCR = 0X00;
}
