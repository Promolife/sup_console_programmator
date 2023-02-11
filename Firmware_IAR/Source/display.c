

void bigdig2(unsigned char x, unsigned char y, unsigned char dig)
{unsigned char dig1=0;
   while(dig >= 10){dig1++;dig -= 10;}  
 digit_size(x,y,dig1);digit_size(x+13,y,dig);}

void bigdig3(unsigned char x, unsigned char y, unsigned int dig)
{ unsigned char dig1=0,dig2=0;
    while(dig >= 100){dig1++;dig -= 100;}
    while(dig >= 10){dig2++;dig -= 10;}
 digit_size(x,y,dig1);digit_size(x+13,y,dig2);digit_size(x+26,y,dig);}

void bigdig4(unsigned char x, unsigned char y, unsigned int dig)
{unsigned char dig1=0,dig2=0,dig3=0;
    while(dig >= 1000){dig1++;dig -= 1000;}
    while(dig >= 100){dig2++;dig -= 100;}
    while(dig >= 10){dig3++;dig -= 10;}
 digit_size(x,y,dig1);digit_size(x+13,y,dig2);digit_size(x+26,y,dig3);digit_size(x+39,y,dig);}

void bigdig5(unsigned char x, unsigned char y, unsigned int dig)
{unsigned char dig1=0,dig2=0,dig3=0,dig4=0;
    while(dig >= 10000){dig1++;dig -= 10000;}
    while(dig >= 1000){dig2++;dig -= 1000;}
    while(dig >= 100){dig3++;dig -= 100;}
    while(dig >= 10){dig4++;dig -= 10;}
 digit_size(x,y,dig1);digit_size(x+13,y,dig2);digit_size(x+26,y,dig3);
 digit_size(x+39,y,dig4);digit_size(x+52,y,dig);}


void adv(unsigned char sec)
{
  PrintStringF(40,0,KRD);
  PrintStringF(15,1,EL);
  PrintStringF(0,2,WWW);
  PrintStringF(0,3,EMAIL);
  for(unsigned char i=0; i < sec; i++){
    delay_s(1);}
}


void statistic()
{
  if(system_flag & DISP_FLAG){
    if((system_flag & MODE_FLAG)){
      
      //READ_KEY(ADMIN1_KEY);
      //dig3(98,0, CHECK_MATCH_KEY());
      //READ_iBUTTON();
  //goto_xy(0,0);
  //printf("%02X%02X%02X%02X%02X%02X%02X%02X",iBUTTON_SERIAL[0],iBUTTON_SERIAL[1],iBUTTON_SERIAL[2],iBUTTON_SERIAL[3],iBUTTON_SERIAL[4],iBUTTON_SERIAL[5],iBUTTON_SERIAL[6],iBUTTON_SERIAL[7]);
  
  PrintStringF(36,0,Time); 
  PrintStringF(0,1,Cycle);dig2(34,1,mT); PrintString(48,1,":"); dig2(54,1,sT);
  PrintStringF(0,2,Ending);dig2(34,2,mD); PrintString(48,2,":"); dig2(54,2,sD);
  PrintStringF(0,3,Fall);dig2(54,3,fail_second);
  PrintStringF(70,0,Day);
  dig3(75,1,counter_day);
  dig3(75,2,counter_dokat_day);
  dig3(75,3,counter_fail_day);}
  PrintStringF(97,0,Total);
  dig5(97,1,counter_total);
  dig5(97,2,counter_dokat);
  dig5(97,3,counter_fail);
  system_flag &= ~DISP_FLAG;}
}

