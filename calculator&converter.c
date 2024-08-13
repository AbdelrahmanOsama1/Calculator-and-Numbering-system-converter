// calculator&converter

// main.c


#define F_CPU 8000000
#include <util/delay.h>

#include "Std_Types.h"
#include "Memory_Map.h"
#include "Utils.h"
#include "DIO_Int.h"
#include "MOTOR_Int.h"
#include "ECU_Int.h"
#include "MOVE_Int.h"
#include "STEPPER_Int.h"
#include "LCD_Int.h"
#include "KEYPAD_Int.h"
#include "Calculator_and_Converter_Int.h"







int main(void)
{

	DIO_Init();
	LCD_Init();
	KEYPAD_Init();
	
	
	
	
	Calculator_and_Converter();
	
	
}


/**************************************************************************************************************************************************************/
/**************************************************************************************************************************************************************/
/**************************************************************************************************************************************************************/
/**************************************************************************************************************************************************************/


// 	Calculator_and_Converter();




#define F_CPU 8000000
#include <util/delay.h>

#include "Std_Types.h"
#include "Memory_Map.h"
#include "Utils.h"
#include "DIO_Int.h"
#include "MOTOR_Int.h"
#include "ECU_Int.h"
#include "MOVE_Int.h"
#include "STEPPER_Int.h"
#include "LCD_Int.h"
#include "KEYPAD_Int.h"




void Calculator_and_Converter(void){
	
	/**********************************************************/
	//general parameters
	
	/**********************************************************/
	u8 received_key;
	u8 start_screen_flag=0;  // =1 if choose app show one time
	
	/**********************************************************/
	//calculator parameters
	/**********************************************************/

	u8 clear_screen_flag=0;              // =1 if screen is empty
	u32 number=0;                        // received_number
	s32 first_operand=0;
	s32 second_operand=0;
	s32 result=0;
	u8 sign;                           // operation sign
	u8 new_sign;                       // next operation sign if not press =
	u8 sign_received_flag=0;           // =1 if operation sign received
	u8 negative_1st_operand=0;         // =1 if 1st operand is negative
	u8 first_operand_exist =0;         // to make sure that user enters number before choose the sign operation
	u8 negative_2nd_operand=0;         // =1 if 2nd operand is negative
	u8 second_operand_exist =0;         // to make sure that user enters '-' sign for 2nd operand (if negative) before writing 2nd operand only
    u8 result_received_flag=0;           // =1 if operation done
	s32 result_float_digits=0;               // get float digits
	s32 integer_result=0;
	u8 first_digit_after_floating_point=0;
	u8 second_digit_after_floating_point=0;
	
	/**********************************************************/
	//converter parameters
	/**********************************************************/
	
	u8 first_choice;
	u8 second_choice;
	u8 first_start_screen_flag = 0 ;  // =1 if user start conv. app
	u8 first_choice_received_flag=0;  // =1 if user choose 1st choice
	u8 second_choice_received_flag=0; // =1 if user choose 2nd choice
	u16 decimal_number=0;   // dec num to convert
	u8 start_receive_num_to_convert_flag=0;    // =1 if user enter the 1st digit of number to convert
	u8 enter_convert_func_flag =0; // =1 if enter this func for first time (to show the start screen for the conversion operation for the first time and not remove)
	u8 convert_result_flag=0; // =1 if convert operation done 
	
	u8 binary_array[8]={0};
	s8 index=7;  // for bin to ... , s8 to make a condition if index < 0
	u8 binary_index=15; // index to write binary from left to right
	
	u8 hex_array[2]={0};
	u8 hex_index=0;
	u8 hex_letters_counter = 'A';  // change between hex letters while entering hex numbers
	u8 hex_second_digit_recevied_flag=0;  // to know if user enter low order digit or not (if enter only 0xF so it = 0x0F not 0xF0)
	u8 hex_temp; // (if enter only 0xF so it = 0x0F not 0xF0) so we must substitute low , high order digits in hex array
	
	/**********************************************************/
	//welcome screen
	/**********************************************************/
	LCD_SetCursor(1,5);
	LCD_WriteStringDynamic("WELCOME");
	LCD_SetCursor(2,1);
	LCD_WriteStringDynamic("AbdelrahmanOsama");
	_delay_ms(1000);
	LCD_ClearDynamic();
	/**********************************************************/
	// choose app
	/**********************************************************/

	while(1){
		
		/**********************************************************/
		// app choose screen
		/**********************************************************/
		if (start_screen_flag==0)
		{
			LCD_SetCursor(1,4);
			LCD_WriteStringDynamic("CHOOSE APP");
			LCD_SetCursor(2,1);
			LCD_WriteStringDynamic("(1)CALC.(2)CONV.");
			start_screen_flag=1;
		}
		
		
	   /**********************************************************/
   	   // receive user choice
 	   /**********************************************************/
		
		received_key = KEYPAD_Getkey();  // get choice

		if (received_key != NO_KEY_PRESSED)  // any button is pressed
		{
			
			
	/***************************************************************************************************************************************************************************************************************************/
	 // calc. app implementation
	/***************************************************************************************************************************************************************************************************************************/			
			if (received_key == '1')  // calc. app
			{
				
				LCD_ClearDynamic();
				clear_screen_flag=1;
				
				while(1){            
					
					
					received_key = KEYPAD_Getkey();  // get numbers for operations

					if (received_key != NO_KEY_PRESSED)  // any button is pressed
					{
						
						if ((received_key>='0')&&(received_key<='9')&&(clear_screen_flag==1)&&(result_received_flag==0))         // receive the 1st digit in 1st operand if positive , make sure that at first (when lcd is cleared) , user can enter only number, will enter here only first time
						{
							clear_screen_flag = 0;
							first_operand_exist=1;
							LCD_WriteChar(received_key);
							number = (number*10) + (received_key-'0');
							
						}else if ((received_key>='0')&&(received_key<='9')&&(clear_screen_flag==0)&&(sign_received_flag==0)&&(result_received_flag==0))    // receive the rest of 1st operand
						{
							first_operand_exist=1;
							LCD_WriteChar(received_key);
							number = (number*10) + (received_key-'0');
							
						}else if ((received_key=='-')&&(clear_screen_flag==1))   // if 1st operand is negative  
						{
							clear_screen_flag=0;
							LCD_WriteChar(received_key);
							negative_1st_operand=1;
							
						}else if (((received_key=='+')||(received_key=='-')||(received_key=='*')||(received_key=='/'))&&(clear_screen_flag==0)&&(sign_received_flag==0)&&(first_operand_exist==1))    // receive the operation sign >>after 1st operand<<
						{
							sign = received_key;
							LCD_WriteChar(sign);
							first_operand = number*100;     // to add 2 digit after float point , to handle float numbers
							sign_received_flag = 1 ;
							number=0;
							
							if (negative_1st_operand ==1)  // if 1st operand is negative
							{
								first_operand = -first_operand;
							}
							
						}else if ((received_key=='-')&&(sign_received_flag==1)&&(second_operand_exist==0)&&(negative_2nd_operand==0))    // if 2nd operand is negative , make sure that user will enter this sign (if 2nd is negative) after operation sign direct only
						{
							
							negative_2nd_operand =1;  // 2nd operand is negative
							LCD_WriteChar(received_key);
							
						}else if ((received_key>='0')&&(received_key<='9')&&(sign_received_flag==1)&&(result_received_flag==0))    // receive 2nd operand
						{
							
							second_operand_exist=1;
							LCD_WriteChar(received_key);
							number = (number*10) + (received_key-'0');
							
							
						}else if ((received_key == '=')&&(first_operand_exist==1)&&(second_operand_exist==1)&&(result_received_flag==0))    // get result
						{
							second_operand = number;
							
							if ((second_operand==0)&&(sign=='/'))   // divide by 0
							{
								LCD_Clear();
								LCD_SetCursor(1,6);
								LCD_WriteString("DON'T");
								LCD_SetCursor(2,2);
								LCD_WriteString("DIVIDE BY ZERO");
								_delay_ms(1000);
								LCD_ClearDynamic();
								
								clear_screen_flag=1;
								number=0;
								first_operand=0;
								second_operand=0;
								result=0;
								sign_received_flag=0;
								negative_1st_operand=0;
								first_operand_exist =0;
								negative_2nd_operand=0;
								second_operand_exist =0;
								result_received_flag=0;
								
							}
							
							else{   // right operation 
								
								
								if (negative_2nd_operand ==1)  // if 2nd operand is negative
								{
									second_operand = -second_operand;
								}
								
								switch(sign){        // perform the operation
									
									case '+':
									second_operand=second_operand*100;  // to add 2 digit after float point , to handle float numbers
									result=first_operand+second_operand;
									break;
									
									case '-':
									second_operand=second_operand*100;  // to add 2 digit after float point , to handle float numbers
									result=first_operand-second_operand;
									break;
									
									case '*':
									result=first_operand*second_operand;
									break;
									
									case '/':
									
									result=first_operand/second_operand;
									
									break;
									
									default:
									// do nothing
									break;
									
								}
								
								result_received_flag=1;
								LCD_SetCursor(2,1);
								LCD_WriteChar('=');
								
								integer_result = result/100;
								result_float_digits=result;
								
								if (result<0)  // if result negative there will be problems in adding float digits
								{
									result_float_digits = -result_float_digits;      // float digits will be 1.-5 if still negative
									LCD_WriteChar('-');
									integer_result=-integer_result;                 // integer result if <1 (0.5) 0 will be (0.5) because int result 0 will not be negative
								}
								
								second_digit_after_floating_point = result_float_digits%10;
								result_float_digits = result_float_digits/10;
								first_digit_after_floating_point=result_float_digits%10;
								
								LCD_WriteNumber(integer_result);
								LCD_WriteChar('.');
								LCD_WriteNumber(first_digit_after_floating_point);
								LCD_WriteNumber(second_digit_after_floating_point);
								
								
							}
							
							
						}else if ((received_key == 'C')&&(clear_screen_flag==0))    // clear screen , set all parameters to its initial state
						{
							LCD_Clear();
							
							clear_screen_flag=1;
							number=0;
							first_operand=0;
							second_operand=0;
							result=0;
							sign_received_flag=0;
							negative_1st_operand=0;
							first_operand_exist =0;
							negative_2nd_operand=0;
							second_operand_exist =0;
							result_received_flag=0;
							
							
						}else if ((received_key>='0')&&(received_key<='9')&&(result_received_flag==1))    // after press = and show result , start new operation not depending on last one
						{
							LCD_Clear();
							LCD_WriteChar(received_key);
							
							clear_screen_flag=0;
							number=0;
							number = (number*10) + (received_key-'0');
							first_operand=0;
							second_operand=0;
							result=0;
							sign_received_flag=0;
							negative_1st_operand=0;
							first_operand_exist =1;
							negative_2nd_operand=0;
							second_operand_exist =0;
							result_received_flag=0;
							
							
						}else if (((received_key=='+')||(received_key=='-')||(received_key=='*')||(received_key=='/'))&&(result_received_flag==1))    // after press = and show result , start new operation but depending on last one
						{
							LCD_Clear();
							
							sign=received_key;
							
							if (result<0)           // if result negative , print the neg. sign  (note that int result became positive)
							{
								LCD_WriteChar('-');
							}
							
							LCD_WriteNumber(integer_result);
							LCD_WriteChar('.');
							LCD_WriteNumber(first_digit_after_floating_point);
							LCD_WriteNumber(second_digit_after_floating_point);
							LCD_WriteChar(sign);
							
							clear_screen_flag=0;
							number=0;
							first_operand=result;
							second_operand=0;
							sign_received_flag=1;
							
							if (result<0)  // 1st new operand is negative
							{
								negative_1st_operand=1;
							}else{
								negative_1st_operand=0;
							}
							
							first_operand_exist =1;
							negative_2nd_operand=0;
							second_operand_exist =0;
							result_received_flag=0;
							result=0;
							
							
						}else if (((received_key=='+')||(received_key=='-')||(received_key=='*')||(received_key=='/'))&&(result_received_flag==0)&&(first_operand_exist==1)&&(second_operand_exist==1))    // enter sign after entering the 2 operands without press =  , start new operation but depending on last one
						{
							LCD_Clear();
							
							second_operand = number;
							
							if ((second_operand==0)&&(sign=='/'))   // divide by 0
							{
								LCD_Clear();
								LCD_SetCursor(1,6);
								LCD_WriteString("DON'T");
								LCD_SetCursor(2,2);
								LCD_WriteString("DIVIDE BY ZERO");
								_delay_ms(1000);
								LCD_ClearDynamic();
								
								clear_screen_flag=1;
								number=0;
								first_operand=0;
								second_operand=0;
								result=0;
								sign_received_flag=0;
								negative_1st_operand=0;
								first_operand_exist =0;
								negative_2nd_operand=0;
								second_operand_exist =0;
								result_received_flag=0;
								
							}
							
							
							else{   // right operation
								
								
								if (negative_2nd_operand ==1)  // if 2nd operand is negative
								{
									second_operand = -second_operand;
								}
								
								switch(sign){        // perform the operation
									
									case '+':
									second_operand=second_operand*100;  // to add 2 digit after float point , to handle float numbers
									result=first_operand+second_operand;
									break;
									
									case '-':
									second_operand=second_operand*100;  // to add 2 digit after float point , to handle float numbers
									result=first_operand-second_operand;
									break;
									
									case '*':
									result=first_operand*second_operand;
									break;
									
									case '/':
									
									result=first_operand/second_operand;
									
									break;
									
									default:
									// do nothing
									break;
									
								}
								
								sign=received_key;
								integer_result = result/100;
								result_float_digits=result;
								
								if (result<0)  // if result negative there will be problems in adding float digits
								{
									result_float_digits = -result_float_digits;      // float digits will be 1.-5 if still negative
									LCD_WriteChar('-');
									integer_result=-integer_result;                 // integer result if <1 (0.5) 0 will be (0.5) because int result 0 will not be negative
								}
								
								
								second_digit_after_floating_point = result_float_digits%10;
								result_float_digits = result_float_digits/10;
								first_digit_after_floating_point=result_float_digits%10;
								
								LCD_WriteNumber(integer_result);
								LCD_WriteChar('.');
								LCD_WriteNumber(first_digit_after_floating_point);
								LCD_WriteNumber(second_digit_after_floating_point);
								
								LCD_WriteChar(sign);
								
								clear_screen_flag=0;
								number=0;
								first_operand=result;
								second_operand=0;
								sign_received_flag=1;
								
								if (result<0)  // 1st new operand is negative
								{
									negative_1st_operand=1;
									}else{
									negative_1st_operand=0;
								}
								
								first_operand_exist =1;
								negative_2nd_operand=0;
								second_operand_exist =0;
								result_received_flag=0;
								result=0;
								
								
							}
							
					
						}else if ((received_key == 'C')&&(clear_screen_flag==1))    // if press C when screen is empty , go to choose app
						{
							start_screen_flag=0;
							break;
							
							
						}
						
						
						
					}
					
					
				}
				
			
				
	/***************************************************************************************************************************************************************************************************************************/
	// conv. app implementation
	/***************************************************************************************************************************************************************************************************************************/
					
			}else if (received_key == '2')  // conv. app
			{
				
				LCD_ClearDynamic();
				
				while(1){             // conv. app implementation
					
					if (first_start_screen_flag == 0)   // enter here 1 time after starting app
					{
						LCD_WriteStringDynamic("CONVERT FROM :");
						LCD_SetCursor(2,1);
						LCD_WriteStringDynamic("1-D  2-B  3-H");
						first_start_screen_flag = 1 ;
					}
					
					
					
					
					if ((first_choice_received_flag==0)||(second_choice_received_flag==0))  // user hasn't chosen 2 choices yet
					{
						
						received_key=KEYPAD_Getkey();
						
						if (received_key != NO_KEY_PRESSED)
						{


							if ((first_choice_received_flag==0)&&(received_key >='1') && (received_key <='3'))  // button pressed for first choice
							{
								
								first_choice = received_key - '0' ;
								first_choice_received_flag=1;
								LCD_ClearDynamic();
								LCD_WriteStringDynamic("CONVERT TO :");
								LCD_SetCursor(2,1);
								LCD_WriteStringDynamic("1-D  2-B  3-H");
								
								
							}else if ((first_choice_received_flag==1)&&(second_choice_received_flag==0)&&(received_key >='1') && (received_key <='3')&&(received_key != first_choice+'0'))  // button pressed for second choice and sec. choice != first choice (can't convert from dec to dec)
							{
								
								second_choice = received_key - '0' ;
								second_choice_received_flag=1;
								LCD_ClearDynamic();
								
							}else if ((received_key =='C')&&(first_choice_received_flag==1))  // back from (convert to ) menu to last menu (convert from ) menu
							{
								LCD_Clear();
								first_start_screen_flag = 0 ;
								first_choice_received_flag=0;
								
							}else if ((received_key =='C')&&(first_choice_received_flag==0))  // back from (convert from ) menu to (choose app) menu
							{
								LCD_Clear();

								first_start_screen_flag = 0 ;
								first_choice_received_flag=0;
								second_choice_received_flag=0;
								decimal_number=0;
								start_receive_num_to_convert_flag=0;
								enter_convert_func_flag =0;
								convert_result_flag=0;
								start_screen_flag=0;
								
								break;
								
							}


						}
					}
					
					
					
					
					if ((first_choice_received_flag==1)&&(second_choice_received_flag==1))   // user chose the 2 choices
					{
						
						if ((first_choice==1)&&((second_choice==2)||(second_choice==3)))                      //*************************************************************************************************** dec to bin/hex
						{
							if (enter_convert_func_flag == 0)   // =1 if enter this func for first time (to show the start screen for the conversion operation for the first time and not remove)
							{
								LCD_WriteString("DEC : ");
								enter_convert_func_flag=1;
							}
							
							received_key=KEYPAD_Getkey();
							
							if (received_key != NO_KEY_PRESSED)
							{
								
								if ((received_key>='0')&&(received_key<='9')&&(convert_result_flag==0))  // enter dec number for 1st time
								{
									start_receive_num_to_convert_flag=1;
									LCD_WriteChar(received_key);
									decimal_number = (decimal_number*10)+(received_key-'0');
									
								}else if ((received_key == '=')&&(start_receive_num_to_convert_flag==1)&&(convert_result_flag==0))  // show convert result
								{
									
									if (decimal_number<=255)
									{
										
										if (second_choice==2)         //*************************************************************************************************** dec to bin
									    {
										LCD_WriteBinary(decimal_number);
										
									    }else if (second_choice==3)   //*************************************************************************************************** dec to hex
									    {
										LCD_WriteHex(decimal_number);
									    }
									
									decimal_number=0;
									convert_result_flag=1;
										
									}else{ // > 255
										
										decimal_number=0;
										
										LCD_Clear();
										LCD_WriteString("NUMBER MUST BE");
										LCD_SetCursor(2,6);
										LCD_WriteString("<= 255");
										_delay_ms(2000);
										LCD_ClearDynamic();
										LCD_WriteString("DEC : ");
										
										
									}
									
									
									
								}else if ((received_key>='0')&&(received_key<='9')&&(convert_result_flag==1))  // after press = , enter new dec numbers to convert to bin/hex
								{
									LCD_Clear();
									LCD_WriteString("DEC : ");
									LCD_WriteChar(received_key);
									decimal_number = (decimal_number*10)+(received_key-'0');
									convert_result_flag=0;
									
								}else if (received_key=='C')  // after press = , back to (convert to) menu
								{
									LCD_Clear();
									second_choice_received_flag=0;
									decimal_number=0;
									start_receive_num_to_convert_flag=0;
									enter_convert_func_flag =0;
									convert_result_flag=0;
									LCD_WriteStringDynamic("CONVERT TO :");
									LCD_SetCursor(2,1);
									LCD_WriteStringDynamic("1-D  2-B  3-H");
									
								}
								
								
							}
							
							
							
						}else if ((first_choice==2)&&((second_choice==1)||(second_choice==3)))                //***************************************************************************************************  bin to dec/hex
						{
							if (enter_convert_func_flag == 0)   // =1 if enter this func for first time (to show the start screen for the conversion operation for the first time and not remove)
							{
								LCD_WriteString("BIN : ");
								enter_convert_func_flag=1;
							}
							
							received_key=KEYPAD_Getkey();
							
							if (received_key != NO_KEY_PRESSED)
							{
								
								if ((received_key>='0')&&(received_key<='1')&&(convert_result_flag==0)&&(index>=0))  // enter bin number for 1st time
								{
									LCD_SetCursor(1,binary_index);
									
									if (binary_index==11)  // put space after 4 bits
									{
										LCD_WriteChar(' ');
										binary_index --; 
										LCD_SetCursor(1,binary_index);
									}
									
									start_receive_num_to_convert_flag=1;
									LCD_WriteChar(received_key);
									binary_array[index] = received_key-'0';
									index--;
									binary_index--;
									
								}else if ((received_key == '=')&&(start_receive_num_to_convert_flag==1)&&(convert_result_flag==0))  // show convert result
								{
									
									if (second_choice==1)         //***************************************************************************************************  bin to dec
									{
										LCD_BinToDec(binary_array);
										
									}else if (second_choice==3)   //*************************************************************************************************** bin to hex
									{
										LCD_BinToHex(binary_array);
									}
									
									for (index=0;index<8;index++)   // reset binary array after show result
									{
										binary_array[index]=0;
									}
									
									index=7;           //  reset index value
									binary_index=15;   //  reset binary index value
									convert_result_flag=1;
									
								}else if ((received_key>='0')&&(received_key<='1')&&(convert_result_flag==1))  // after press = , enter new bin numbers to convert to dec/hex
								{
									LCD_Clear();
									LCD_WriteString("BIN : ");
									LCD_SetCursor(1,binary_index);
									LCD_WriteChar(received_key);
									binary_array[index] = received_key-'0';
									index--;
									binary_index--;
									convert_result_flag=0;
									
								}else if (received_key=='C')  // after press = , back to (convert to) menu
								{
									LCD_Clear();
									second_choice_received_flag=0;
									for (index=0;index<8;index++)   // reset binary array after show result
									{
										binary_array[index]=0;
									}
									
									index=7;  //  reset index value
									binary_index=15;   //  reset binary index value
									start_receive_num_to_convert_flag=0;
									enter_convert_func_flag =0;
									convert_result_flag=0;
									
									LCD_WriteStringDynamic("CONVERT TO :");
									LCD_SetCursor(2,1);
									LCD_WriteStringDynamic("1-D  2-B  3-H");
									
								}
								
								
							}
							
							
							
						}else if ((first_choice==3)&&((second_choice==2)||(second_choice==1)))                //*************************************************************************************************** hex to bin/dec
						{
							if (enter_convert_func_flag == 0)   // =1 if enter this func for first time (to show the start screen for the conversion operation for the first time and not remove)
							{
								LCD_WriteString("HEX : 0x");
								enter_convert_func_flag=1;
							}
							
							received_key=KEYPAD_Getkey();
							
							if (received_key != NO_KEY_PRESSED)
							{
								
								if ((received_key>='0')&&(received_key<='9')&&(convert_result_flag==0)&&(hex_index==0))  // enter 1st digit for hex number for 1st time (0:9)
								{
									hex_letters_counter = 'A';
									LCD_SetCursor(1,9);
									start_receive_num_to_convert_flag=1;
									LCD_WriteChar(received_key);
									hex_array[hex_index] = received_key;
									
									
								}else if ((received_key=='+')&&(convert_result_flag==0)&&(hex_index==0))  // enter 1st digit for hex number for 1st time (A:F)
								{
									start_receive_num_to_convert_flag=1;
									LCD_SetCursor(1,9);
									LCD_WriteChar(hex_letters_counter);
									hex_array[hex_index] = hex_letters_counter;
									hex_letters_counter++;
									
									if (hex_letters_counter > 'F')  // make sure that hex letters from 'A' to 'F'
									{
										hex_letters_counter = 'A';
									}
									
									
					
									
							   }else if ((received_key=='*')&&(start_receive_num_to_convert_flag==1)&&(hex_index==0))  // move to 2nd digit
						       {
								hex_index++;
								hex_letters_counter = 'A';
								
								
							   }else if ((received_key>='0')&&(received_key<='9')&&(convert_result_flag==0)&&(hex_index==1))  // enter 2nd digit for hex number for 1st time (0:9)
							   {
								    hex_letters_counter = 'A';
								    hex_second_digit_recevied_flag=1;
									LCD_SetCursor(1,10);
									LCD_WriteChar(received_key);
									hex_array[hex_index] = received_key;
									
									
							   }else if ((received_key=='+')&&(convert_result_flag==0)&&(hex_index==1))  // enter 2nd digit for hex number for 1st time (A:F)
							   {
								    hex_second_digit_recevied_flag=1;
									LCD_SetCursor(1,10);
									LCD_WriteChar(hex_letters_counter);
									hex_array[hex_index] = hex_letters_counter;
									hex_letters_counter++;
									
									if (hex_letters_counter > 'F')  // make sure that hex letters from 'A' to 'F'
									{
										hex_letters_counter = 'A';
									}
									
									
					
									
						   	   }else if ((received_key == '=')&&(start_receive_num_to_convert_flag==1)&&(convert_result_flag==0))  // show convert result 
						 	   {
								
								
								   if (hex_second_digit_recevied_flag==0)  // if user enter only one digit (this will be low order not high)
								   {
									   hex_temp = hex_array[0];
									   hex_array[0]=hex_array[1];
									   hex_array[1]=hex_temp;
								   }
									
									if (second_choice==1)         //*************************************************************************************************** hex to dec
									{
										LCD_HexToDec(hex_array);
										
									}else if (second_choice==2)   //*************************************************************************************************** hex to bin
									{
										LCD_HexToBIN(hex_array);
									}
									
									
									for (hex_index=0;hex_index<2;hex_index++)   // reset hex array after show result
									{
										hex_array[hex_index]=0;
									}
									
									hex_index=0;
									hex_letters_counter = 'A';
									hex_second_digit_recevied_flag=0;
									convert_result_flag=1;
									
								}else if ((received_key>='0')&&(received_key<='9')&&(convert_result_flag==1))  // after press = , enter new hex numbers to convert to bin/dec
								{
									LCD_Clear();
									LCD_WriteString("HEX : 0x");
									LCD_WriteChar(received_key);
									hex_array[hex_index] = received_key;
									convert_result_flag=0;
									
								}else if ((received_key=='+')&&(convert_result_flag==1))  // after press = , enter new hex numbers to convert to bin/dec
								{
									LCD_Clear();
									LCD_WriteString("HEX : 0x");
									LCD_WriteChar(hex_letters_counter);
									hex_array[hex_index] = hex_letters_counter;
									hex_letters_counter++;
									convert_result_flag=0;
									
								}else if (received_key=='C')  // after press = , back to (convert to) menu
								{
									LCD_Clear();
									
									for (hex_index=0;hex_index<2;hex_index++)   // reset hex array after show result
									{
										hex_array[hex_index]=0;
									}
									
									hex_index=0;
									hex_letters_counter = 'A';
									hex_second_digit_recevied_flag=0;
									convert_result_flag=1;
									second_choice_received_flag=0;
									start_receive_num_to_convert_flag=0;
									enter_convert_func_flag =0;
									convert_result_flag=0;
									
									LCD_WriteStringDynamic("CONVERT TO :");
									LCD_SetCursor(2,1);
									LCD_WriteStringDynamic("1-D  2-B  3-H");
									
								}
								
								
							}
							
							
							
						}
						
						
					}//user chose 2 choices
					
			
				} //while

			}//else if (received_key == '2')
			
			
		} // if (received_key != NO_KEY_PRESSED)
		
	}//outer loop
	
	
}//func



/**************************************************************************************************************************************************************/
/**************************************************************************************************************************************************************/
/**************************************************************************************************************************************************************/
/**************************************************************************************************************************************************************/