
#include "ui.h"


#define APP_NBR 6
#define PREFACE_SELECT_DELAY 25


const char* week_str[] = {"Mon","Tue","Wed","Thr","Fri","Sat","Sun"};
 
typedef enum
{
	RESERVE = 0,
	APP_RTC = 1,
	APP_STW,
	APP_SET,
	APP_AHRS,
	APP_GAME,
	APP_ENV
}index_e;


typedef enum
{
	PREFACE = 0,
	APP
}menu_mode_e;



typedef enum
{
	DETECT_STA = 0,  //app detect vibration status
	ENTER_STA,       //app enter   status
    RUN_STA,         //app running status
	STBY_STA,        //app standby status
	EXIT_STA         //app exit    status
}app_sta_e;

void app_preface(Data data);
void app_rtc(Data data);
void app_stw(Data data);
void app_set(Data data);
void app_ahrs(Data data);
void app_game(Data data);
void app_env(Data data);


typedef	void   (*excute_t) (Data data);


typedef struct
{
	index_e         index;
	menu_mode_e     mode;
	excute_t        preface_excute;
	excute_t        app_excute[APP_NBR+1];
}Menu;

Menu menu=
{
		.index = APP_RTC,
		.mode  = APP,                     //Preface or app show
		.preface_excute = app_preface,    //functions
	    .app_excute[APP_RTC] = app_rtc,
		.app_excute[APP_STW] = app_stw,
    	.app_excute[APP_SET] = app_set,
		.app_excute[APP_AHRS] = app_ahrs,
		.app_excute[APP_GAME] = app_game,
		.app_excute[APP_ENV] = app_env
}; 


/**
  * @brief  app 1,shows temperture, battery info, steps, rtc 
  * @param  data: ui data structure
  * @retval none 
  * @date   2022-01-26
  */
void app_rtc(Data data)
{
	char buf[50] = {0};
	static app_sta_e  app_sta = ENTER_STA;
    static uint8_t sec_unit_last;
    uint8_t sec_unit = 0 , sec_ten = 0, min_unit = 0, min_ten = 0, hour_unit = 0, hour_ten = 0;
    static  FlagStatus sec_move_flag;
    static  int8_t sec_unit_move;
    static  int8_t sec_ten_move;

    //entrance animation
    static int8_t  animation_cnt0 = -60; //initialize the string position
	static int8_t  animation_cnt1 = 130;
	static int8_t  animation_cnt2 = -10;

	static uint8_t animation_delay0;     //determine the string movement speed
	static uint8_t animation_delay1;
	static uint8_t animation_delay2;

	static uint8_t seq = 0;               //appear sequence   seq=0/1 (the first/second string shows)

    sec_ten = data.sec/10;
    sec_unit = data.sec%10;
    min_ten = data.min/10;
    min_unit = data.min%10;
    hour_ten = data.hour/10;
    hour_unit = data.hour%10;

	if(QUIT)
	{
		seq = 0;
	    animation_cnt0 = -60;
	    animation_cnt1 = 150;
	    animation_cnt2 = -10;
    	sec_ten_move = 0;
    	sec_unit_move = 0;

		menu.mode = PREFACE;
		app_sta = ENTER_STA;
	}


	if(app_sta==ENTER_STA)
	{
		if(seq==0)
		{
			if(++animation_delay0==1)
			{
				animation_delay0 = 0;
				animation_cnt0 += 2;
				if(animation_cnt0==4)
				{
					seq = 1;
			    }

			}
		}

	    else if(seq==1)
		{
			if(++animation_delay1==2)
			{
				animation_delay1 = 0;
				if(--animation_cnt1==120)
				{
					seq = 2;
				}

			}

		}

	    else if(seq==2)
		{
			if(++animation_delay2==2)
			{
				animation_delay2 = 0;
				if(++animation_cnt2==9)
				{
					seq = 3;
				}

			}
		}



       if(seq==3)
		{
            seq = 0;
            app_sta = RUN_STA;
		}
	}



	if(app_sta==RUN_STA)
	{

		if(data.pitch>50)
		{
			u8g2_SetPowerSave(&u8g2, 1);

	        HAL_NVIC_EnableIRQ(MEMS_IT_EXTI_IRQn);
	        HAL_NVIC_DisableIRQ(INTERRUPT_TIM_IRQN);
	        HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	        //system stop from here

	        HAL_NVIC_DisableIRQ(MEMS_IT_EXTI_IRQn);
	        SystemClock_Config();
	        HAL_Delay(200);


            u8g2_SetPowerSave(&u8g2, 0);
	        HAL_NVIC_EnableIRQ(INTERRUPT_TIM_IRQN);

		}

        if(sec_unit!=sec_unit_last)
        {
        	sec_move_flag = SET;
        	sec_ten_move = 0;
        	sec_unit_move = 0;
        }



        sec_unit_last = sec_unit;

        if(sec_move_flag==SET)
        {
        	if(sec_ten==1)
        	{
        		if(++sec_unit_move==18)
        			sec_unit_move = 17;
        	}
        	else
        	{
        		if(++sec_unit_move==11)
        			sec_unit_move = 10;
        	}


        	if(++sec_ten_move==20)
        	   sec_move_flag = RESET;
        }


	}




    u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);
    u8g2_DrawStr(&u8g2,animation_cnt0-3,55,"L-BAND");


    sprintf(buf,"%02d%02d",data.month,data.date);
    u8g2_DrawStr(&u8g2,5-animation_cnt0,85,buf);



	sprintf(buf,"20%02d",data.year);
    u8g2_DrawStr(&u8g2,5-animation_cnt0+10,75,buf);



    sprintf(buf,"%d",sec_ten);
    u8g2_DrawStr(&u8g2,64-sec_ten_move,95,buf);

    sprintf(buf,"%d",sec_unit);
    u8g2_DrawStr(&u8g2,64-sec_unit_move,95,buf);



    sprintf(buf,"%d",min_ten);
    u8g2_DrawStr(&u8g2,26-animation_cnt0,95,buf);

    sprintf(buf,"%d",min_unit);
    u8g2_DrawStr(&u8g2,36-animation_cnt0,95,buf);

    sprintf(buf,"%d",hour_ten);
    u8g2_DrawStr(&u8g2,4-animation_cnt0,95,buf);

    sprintf(buf,"%d",hour_unit);
    u8g2_DrawStr(&u8g2,14-animation_cnt0,95,buf);


    if(hour_unit==1)
    	u8g2_DrawStr(&u8g2, 22-animation_cnt0, 95, ":");


    else
    	u8g2_DrawStr(&u8g2, 24-animation_cnt0, 95, ":");



    if(min_unit==1)
    	u8g2_DrawStr(&u8g2, 44-animation_cnt0, 95, ":");


    else
        u8g2_DrawStr(&u8g2, 46-animation_cnt0, 95, ":");





    sprintf(buf,"%d",data.step);
    u8g2_DrawStr(&u8g2,20,animation_cnt1,buf);

    u8g2_SetFont(&u8g2,u8g2_font_open_iconic_human_2x_t);
    u8g2_DrawGlyph(&u8g2,0,animation_cnt1,68);


    u8g2_SetFont(&u8g2,u8g2_font_blipfest_07_tr);
    sprintf(buf,"%.1fC                            %.1fV",data.temperature,data.batt);
    u8g2_DrawStr(&u8g2,0,animation_cnt2,buf);


    sprintf(buf,"%s",week_str[data.week-1]);
    u8g2_DrawStr(&u8g2,49-animation_cnt0,85,buf);






	
}

/**
  * @brief  app 2,shows stopwatch 
  * @param  data: ui data structure
  * @retval none 
  * @date   2022-01-26
  */
void app_stw(Data data)
{
    #define CIRCLE_SEC_X 20
    #define CIRCLE_SEC_Y 40
    #define CIRCLE_SEC_R 8

    #define CIRCLE_MSEC_X 50
    #define CIRCLE_MSEC_Y 40
    #define CIRCLE_MSEC_R 8

	char              buf[20];
	static app_sta_e  app_sta = ENTER_STA;

	static uint32_t   previous_time;
	static uint32_t   delta;
	static uint8_t   min;
	static uint8_t   sec;
	static uint32_t   msec;
	static uint32_t   time_base;
	
	static FlagStatus cnt_flag;       //timming turn on or off
    static uint8_t    delay_cnt;      //interval between 2 vibration detection
	static uint8_t    cnt_mode = 0;   //0:LAP  1:SPL

	static int8_t     spl_player;//max: 5 players
	static uint8_t    spl_min[5] = {0};
	static uint8_t    spl_sec[5] = {0};
	static uint32_t    spl_msec[5] = {0};
    int8_t x0,y0;

    //animation
    static int8_t  animation_cnt0 = 0; //initialize the string position
    static int8_t  animation_cnt1 = 64;
    static int8_t  animation_cnt2 = 0;

    static uint8_t animation_delay0,animation_delay1,animation_delay2;     //determine the string movement speed
    static uint8_t seq = 0;               //appear sequence   seq=0/1 (the first/second string shows)

	if(QUIT)
	{
		min = 0;
		sec = 0;
		msec = 0;
		time_base = 0;
		delta = 0;
		seq = 0;
		menu.mode = PREFACE;
		app_sta = ENTER_STA;
		cnt_flag = RESET;
		cnt_mode = 0;
      	animation_cnt0 = 0;
      	animation_cnt1 = 64;
      	animation_cnt2 = 0;
	}
    if(app_sta==ENTER_STA)
    {
    	if(seq==0)
      	{
    		if(++animation_delay0==1)
      	    {
    			animation_delay0 = 0;

      			if(++animation_cnt0==25)
      			{
      				seq = 1;
      			}

      		}
      	}

       else if(seq==1)
    	{
    		if(++animation_delay1==1)
    		{
    			animation_delay1 = 0;
    			animation_cnt1 -= 2;
    			if(animation_cnt1==0)
    			{
    				seq = 2;
    			}

    		}

    	}

       else if(seq==2)
      	{
    		if(++animation_delay2==5)
      	    {
    			animation_delay2 = 0;

      			if(++animation_cnt2==CIRCLE_SEC_R)
      			{
      				seq = 3;
      			}



      		}



  	        u8g2_DrawCircle(&u8g2,CIRCLE_SEC_X,CIRCLE_SEC_Y,animation_cnt2,U8G2_DRAW_ALL);
  	        x0 = CIRCLE_SEC_X + (int8_t)(FastSin(RADTODEG(animation_cnt2)*20.f)*animation_cnt2);
  	        y0 = CIRCLE_SEC_Y - (int8_t)(animation_cnt2*FastCos(RADTODEG(animation_cnt2)*20.f));
  	        u8g2_DrawLine(&u8g2,CIRCLE_SEC_X, CIRCLE_SEC_Y, x0, y0);

  	        u8g2_DrawCircle(&u8g2,CIRCLE_MSEC_X,CIRCLE_MSEC_Y,animation_cnt2,U8G2_DRAW_ALL);
  	        x0 += CIRCLE_MSEC_X - CIRCLE_SEC_X;
  	        y0 += CIRCLE_MSEC_Y - CIRCLE_SEC_Y;
  	        u8g2_DrawLine(&u8g2,CIRCLE_MSEC_X, CIRCLE_MSEC_Y, x0, y0);





      	}





    	if(seq==3)
    	{
            seq = 0;

          	app_sta = RUN_STA;
    	}

      	sprintf(buf,"+%02d : %02d",min,sec);
      	u8g2_DrawStr(&u8g2,animation_cnt1,60,buf);
      	sprintf(buf,"%03ld",msec);
      	u8g2_DrawStr(&u8g2,20+animation_cnt1,70,buf);

    }

	u8g2_DrawStr(&u8g2,0,animation_cnt0,"LAP");

	if(app_sta==RUN_STA)
	{
		if(data.dir==LEFT)
		{
			cnt_mode = 0;       //LAP
			cnt_flag = RESET;

		    delta = 0;
		    time_base = 0;
		}
		
		if(data.dir==RIGHT)
		{
			cnt_mode = 1;       //SPL
			cnt_flag = RESET;
			spl_player = -1;


		    delta = 0;
		    time_base = 0;
			memset(spl_min,0,sizeof(spl_min));
			memset(spl_sec,0,sizeof(spl_sec));
            memset(spl_msec,0,sizeof(spl_msec));

		}

		//start/stop counting
		if(VIB)
	    {
			app_sta = STBY_STA;

			//LAP
		    if(cnt_mode==0)
		    	cnt_flag = (FlagStatus)!cnt_flag;

		    //SPL
		    if(cnt_mode==1&&cnt_flag==SET)
		    {
		    	if(++spl_player==5)
			    {
		    		spl_player = 0;
				    memset(spl_min,0,sizeof(spl_min));
			        memset(spl_sec,0,sizeof(spl_sec));
                    memset(spl_msec,0,sizeof(spl_msec));
			    }
			 
			    spl_min[spl_player] = min;
			    spl_sec[spl_player] = sec;
                spl_msec[spl_player] = msec;
		    }


		    if(cnt_mode==1 && cnt_flag==RESET) //SPL
		    {
		    	cnt_flag = SET;
		    	return;
            }
	     }

	}
	 
 	if(app_sta==STBY_STA)
	{
		if(++delay_cnt==20)
		{
			delay_cnt = 0;
			app_sta = RUN_STA;
		}
	}


    if(cnt_flag==RESET)//timing stop
    	previous_time = tick;

    else
    {
    	delta = tick-previous_time;
    	previous_time = tick;
    }

     time_base += delta;
     delta = 0;

     min = time_base/1000/60;
     sec = time_base/1000%60;

     msec = time_base-sec*1000-min*60*1000;


	
	u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);


	
	
	if(cnt_mode==1)
	  u8g2_DrawStr(&u8g2,0,25,"SPL");
	


	if(app_sta!=ENTER_STA)
    {
		sprintf(buf,"+%02d : %02d",min,sec);
        u8g2_DrawStr(&u8g2,0,60,buf);

	    sprintf(buf,"%03ld",msec);
        u8g2_DrawStr(&u8g2,20,70,buf);

       //stw meter
       u8g2_DrawCircle(&u8g2,CIRCLE_SEC_X,CIRCLE_SEC_Y,CIRCLE_SEC_R,U8G2_DRAW_ALL);
       x0 = CIRCLE_SEC_X + (int8_t)(FastSin(DEGTORAD(sec)*6.0f)*CIRCLE_SEC_R);
       y0 = CIRCLE_SEC_Y - (int8_t)(CIRCLE_SEC_R*FastCos(DEGTORAD(sec)*6.0f));
       u8g2_DrawLine(&u8g2,CIRCLE_SEC_X, CIRCLE_SEC_Y, x0, y0);

       u8g2_DrawCircle(&u8g2,CIRCLE_MSEC_X,CIRCLE_MSEC_Y,CIRCLE_MSEC_R,U8G2_DRAW_ALL);
       x0 = CIRCLE_MSEC_X + (int8_t)(FastSin(DEGTORAD(msec)*0.36f)*CIRCLE_MSEC_R);
       y0 = CIRCLE_MSEC_Y - (int8_t)(CIRCLE_MSEC_R*FastCos(DEGTORAD(msec)*0.36f));
       u8g2_DrawLine(&u8g2,CIRCLE_MSEC_X, CIRCLE_MSEC_Y, x0, y0);
    }


	
  if(cnt_mode==1)
  {
		//with each times of vibration,the time of 5 players will be recorded in sequence
	  for(uint8_t i=0;i<=4;i++)
	  {
		  if(spl_player>=i)
    	  {
			  sprintf(buf,"%01d:%02d:%03ld",spl_min[i],spl_sec[i],spl_msec[i]);
			  u8g2_DrawStr(&u8g2,0,85+i*10,buf);
		  }
	  }
  }
	

}



/**
  * @brief  app 3,set rtc,alarm value
  * @param  data: ui data structure
  * @retval none 
	* @date   2022-02-02
  */
void app_set(Data data)
{
  char   buf[20] =  {0};
  static int8_t time_set[10] = {0};  //rtc[0:5] year/month/week/date/hour/min/sec    alm[6:8] hour/min/sec
  static int8_t     display_buf[10] = {0};

  static app_sta_e  app_sta = ENTER_STA;
  
  static uint8_t    delay_cnt;       //interval between 2 vibrate detections
                                   	 //interval between 2 operations
  static uint8_t   flash_cnt;
  static uint8_t   flash_flag;
  static int8_t    choose;
  static FlagStatus   time_import_flag = RESET;

  //animation
  static int8_t  animation_cnt0 = 64; //initialize the string position
  static int8_t  animation_cnt1 = -20;


  static uint8_t animation_delay0,animation_delay1;     //determine the string movement speed

  static uint8_t seq = 0;               //appear sequence   seq=0/1 (the first/second string shows)

  if(app_sta==ENTER_STA)
  {
		if(time_import_flag==RESET)
		{
		  time_set[0] = data.year;
		  time_set[1] = data.month;
		  time_set[2] = data.date;
		  time_set[3] = data.week;
	      time_set[4] = data.hour;
	      time_set[5] = data.min;
		  time_set[6] = data.sec;
		  time_set[7] = alarm[0];
		  time_set[8] = alarm[1];
		  time_set[9] = alarm[2];

		  time_import_flag = SET;

			for(uint8_t i = 0;i<10;i++)
			  display_buf[i] = time_set[i];
		 }


  		if(seq==0)
  		{
  			if(++animation_delay0==1)
  			{
  				animation_delay0 = 0;
  				animation_cnt0 -= 2;
  				if(animation_cnt0==0)
  				{
  					seq = 1;
  				}

  			}

  		}

  	    else if(seq==1)
  		{


  			if(++animation_delay1==1)
  			{
  				animation_delay1 = 0;

  				if(++animation_cnt1==0)
  				{
  					seq = 2;
  			    }

  			}

  		}

  		if(seq==2)
  		{
            seq = 0;
          	animation_cnt0 = 64;
          	animation_cnt1 = -20;
          	app_sta = RUN_STA;
  		}




        u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);

        sprintf(buf,"20%02d",display_buf[0]);
        u8g2_DrawStr(&u8g2,animation_cnt0,40,buf);

 		sprintf(buf,"%02d%02d %d",display_buf[1],display_buf[2],display_buf[3]);
        u8g2_DrawStr(&u8g2,animation_cnt0,50,buf);

 		sprintf(buf,"%02d:%02d:%02d",display_buf[4],display_buf[5],display_buf[6]);
        u8g2_DrawStr(&u8g2,animation_cnt0,60,buf);

        sprintf(buf,"%02d:%02d:%02d",display_buf[7],display_buf[8],display_buf[9]);
        u8g2_DrawStr(&u8g2,animation_cnt0,100,buf);


 		u8g2_SetFont(&u8g2,u8g2_font_open_iconic_app_2x_t);
        u8g2_DrawGlyph(&u8g2,animation_cnt1,30,69);
 		u8g2_SetFont(&u8g2,u8g2_font_open_iconic_embedded_2x_t);
        u8g2_DrawGlyph(&u8g2,animation_cnt1,90,65);




  }

  if(app_sta==RUN_STA)
	{
		
		if(++delay_cnt==40)
		{
			delay_cnt = 0;
		  
			switch(data.dir)
			{
				case RIGHT:     if(++choose==10)
					                choose = 9;
				                 break;
			
		  
			    case LEFT:      if(--choose==-1)
				                   choose = 0;
							    break;

			    case FRWD:      time_set[choose]++;
			                    if(choose==8 || choose==9 ||choose==5 ||choose==6)//min,sec
								{
									if(time_set[choose]>59)
										time_set[choose] = 59;
								}
								else if(choose==4 || choose==7)//hour
			                    {
									if(time_set[choose]>23)
									   time_set[choose] = 23;
								}
												
								else if(choose==1)//month
			                    {
									if(time_set[choose]>12)
										time_set[choose] = 12;
								}
												
							    else if(choose==2)//date
			                    {
									if(time_set[choose]>31)
										time_set[choose] = 31;
								}
								else if(choose==0)//year
			                    {
									if(time_set[choose]>99)
										time_set[choose] = 99;
								}
							    else if(choose==3)//week
			                    {
								   if(time_set[choose]>7)
									   time_set[choose] = 7;
								}
							    else ;

			                    break;
		   
			  
				case BKWD:      time_set[choose]--; 
			                 	if(choose==1 || choose==2 || choose==3)//month and date
                                {
									if(time_set[choose]==0)
										time_set[choose] = 1;
								}
								else
								{

									if(time_set[choose]<0)
										time_set[choose] = 0;

								}
							    break;
				
				default:        break;
			}
		}
		
		for(uint8_t i = 0;i<10;i++)
		  display_buf[i] = time_set[i];
		
		
		if(++flash_cnt==30)
		{
			flash_cnt = 0;
			flash_flag = !flash_flag;
		}
		
		if(flash_flag)
			display_buf[choose] = time_set[choose];
		else
		  display_buf[choose] = 0;


		//icon
		u8g2_SetFont(&u8g2,u8g2_font_open_iconic_app_2x_t);
        u8g2_DrawGlyph(&u8g2,0,30,69);
		u8g2_SetFont(&u8g2,u8g2_font_open_iconic_embedded_2x_t);
        u8g2_DrawGlyph(&u8g2,0,90,65);


        u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);

        sprintf(buf,"20%02d",display_buf[0]);
        u8g2_DrawStr(&u8g2,0,40,buf);

		sprintf(buf,"%02d%02d %d",display_buf[1],display_buf[2],display_buf[3]);
        u8g2_DrawStr(&u8g2,0,50,buf);

		sprintf(buf,"%02d:%02d:%02d",display_buf[4],display_buf[5],display_buf[6]);
        u8g2_DrawStr(&u8g2,0,60,buf);

        sprintf(buf,"%02d:%02d:%02d",display_buf[7],display_buf[8],display_buf[9]);
        u8g2_DrawStr(&u8g2,0,100,buf);

        if(VIB) //save
        {
			sDate.Year    = time_set[0];
			sDate.Month   = time_set[1];
			sDate.Date    = time_set[2];
			sDate.WeekDay = time_set[3];
			sTime.Hours   = time_set[4];
			sTime.Minutes = time_set[5];
            sTime.Seconds	= time_set[6];
      
			HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
            HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
			alarm[0] = time_set[7];			
			alarm[1] = time_set[8];			
            alarm[2] = time_set[9];
      
			app_sta = EXIT_STA;
		  
	    }
	}

	if(app_sta==EXIT_STA)
	{
		u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);
        u8g2_DrawStr(&u8g2,0,60,"SET OK!");
		
    	if(QUIT)
    	{
    		seq = 0;
    	    animation_cnt0 = 64;
    	    animation_cnt1 = -20;

    		menu.mode = PREFACE;
    		app_sta = ENTER_STA;
    	}
	}
}

/** 
  * @brief  app 4,shows ahrs data
  * @param  data: ui data structure
  * @retval none 
  * @date   2022-02-02
  */

void app_ahrs(Data data)
{
	#define CIRCLE_ROLL_X 40
	#define CIRCLE_ROLL_Y 13
	#define CIRCLE_ROLL_R 13
	
	#define CIRCLE_PITCH_X 40
	#define CIRCLE_PITCH_Y 53
	#define CIRCLE_PITCH_R 13
	
	#define CIRCLE_YAW_X 40
	#define CIRCLE_YAW_Y 93
	#define CIRCLE_YAW_R 13

	char buf[10];
    int16_t     x0,y0;
    static app_sta_e  app_sta = ENTER_STA;

    //animation
    static int8_t  animation_cnt0 = 0; //initialize the string position
    static int8_t  animation_cnt1 = -10;

    static uint8_t animation_delay0,animation_delay1;
    static uint8_t seq = 0;               //appear sequence   seq=0/1 (the first/second string shows)

    if(app_sta==ENTER_STA)
    {
    	if(seq==0)
      	{
    		if(++animation_delay0==1)
      	    {
    			animation_delay0 = 0;

      			if(++animation_cnt0==40)
      			{
      				seq = 1;
      			}

      		}
      	}

       else if(seq==1)
    	{
    		if(++animation_delay1==1)
    		{
    			animation_delay1 = 0;

    			if(++animation_cnt1==0)
    			{
    				seq = 2;
    			}

    		}

    	}






    	if(seq==2)
    	{
            seq = 0;

          	app_sta = RUN_STA;
    	}


    	u8g2_uint_t r = (u8g2_uint_t)((float)animation_cnt0*0.325f);
	    u8g2_DrawCircle(&u8g2,CIRCLE_ROLL_X,53-animation_cnt0,r,U8G2_DRAW_ALL);
	    u8g2_DrawCircle(&u8g2,CIRCLE_PITCH_X,CIRCLE_PITCH_Y,r,U8G2_DRAW_ALL);
        u8g2_DrawCircle(&u8g2,CIRCLE_YAW_X,53+animation_cnt0,r,U8G2_DRAW_ALL);





        u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);

		u8g2_DrawStr(&u8g2,animation_cnt1,20,"X");


		u8g2_DrawStr(&u8g2,animation_cnt1,56,"Y");


		u8g2_DrawStr(&u8g2,animation_cnt1,96,"Z");


    }
	
	if(app_sta==RUN_STA)
	{
		u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);
	  
		//roll 
		u8g2_DrawStr(&u8g2,0,20,"X");
        sprintf(buf,"%d",(int16_t)data.roll);
        u8g2_DrawStr(&u8g2,35,35,buf);
		//roll meter
	    u8g2_DrawCircle(&u8g2,CIRCLE_ROLL_X,CIRCLE_ROLL_Y,CIRCLE_ROLL_R,U8G2_DRAW_ALL);
	    x0 = CIRCLE_ROLL_X + (int8_t)(FastSin(DEGTORAD(data.roll)*6.0f)*CIRCLE_ROLL_R);
	    y0 = CIRCLE_ROLL_Y - (int8_t)(CIRCLE_ROLL_R*FastCos(DEGTORAD(data.roll)*6.0f));
        u8g2_DrawLine(&u8g2,CIRCLE_ROLL_X, CIRCLE_ROLL_Y, x0, y0);
		
		//pitch
		u8g2_DrawStr(&u8g2,0,56,"Y");
        sprintf(buf,"%d",(int16_t)data.pitch);
        u8g2_DrawStr(&u8g2,35,75,buf);
		//pitch meter
		u8g2_DrawCircle(&u8g2,CIRCLE_PITCH_X,CIRCLE_PITCH_Y,CIRCLE_PITCH_R,U8G2_DRAW_ALL);
	    x0 = CIRCLE_PITCH_X + (int8_t)(FastSin(DEGTORAD(data.pitch)*6.0f)*CIRCLE_PITCH_R);
	    y0 = CIRCLE_PITCH_Y - (int8_t)(CIRCLE_PITCH_R*FastCos(DEGTORAD(data.pitch)*6.0f));
        u8g2_DrawLine(&u8g2,CIRCLE_PITCH_X, CIRCLE_PITCH_Y, x0, y0);
		
		//yaw 
		u8g2_DrawStr(&u8g2,0,96,"Z");
        sprintf(buf,"%d",(int16_t)data.yaw);
        u8g2_DrawStr(&u8g2,35,115,buf);
		//yaw meter
		u8g2_DrawCircle(&u8g2,CIRCLE_YAW_X,CIRCLE_YAW_Y,CIRCLE_YAW_R,U8G2_DRAW_ALL);
	    x0 = CIRCLE_YAW_X + (int8_t)(FastSin(DEGTORAD(data.yaw))*CIRCLE_YAW_R);
	    y0 = CIRCLE_YAW_Y - (int8_t)(CIRCLE_YAW_R*FastCos(DEGTORAD(data.yaw)));
        u8g2_DrawLine(&u8g2,CIRCLE_YAW_X, CIRCLE_YAW_Y, x0, y0);
	   

		
		if(VIB) 
		  app_sta = EXIT_STA;
	}
	
	
	if(app_sta==EXIT_STA)
	{
		u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);
        u8g2_DrawStr(&u8g2,0,60,"SET OK!");
		
		if(QUIT)
		{
			menu.mode = PREFACE;
			app_sta = ENTER_STA;
			animation_cnt0 = 0;
			animation_cnt1 = -10;

			seq = 0;
		}
	}
}




/**
  * @brief  app 6,game
  * @param  data: ui data structure
  * @retval none 
	* @date   2022-02-12
  */
void app_game(Data data)
{ 
  static app_sta_e  app_sta = DETECT_STA;
  static uint8_t    pts = 0;
  char buf[10] = {0};
  static int8_t  animation_cnt0 = -64;
  static uint8_t animation_delay0;

	
  if(app_sta==DETECT_STA)
  {
	  if(VIB)
		  app_sta = RUN_STA;


	  if(++animation_delay0==1)
	  {
		  animation_delay0 = 0;
		  animation_cnt0 += 2;
		  if(animation_cnt0==2)
			  animation_cnt0 = 0;

	  }


	  u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);
	  u8g2_DrawStr(&u8g2,animation_cnt0,60,"GREEDY");
	  u8g2_DrawStr(&u8g2,5-animation_cnt0,70,"SNAKE");
  }


  else if(app_sta==RUN_STA)
  {
	  if(game_excute(data.dir,&pts)==SUCCESS)
	  {
		  sprintf(buf,"%d pts",pts);
		  u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);
		  u8g2_DrawStr(&u8g2,0,120,buf);
	  }
	  else
		  app_sta = EXIT_STA;
  }
		
  else if(app_sta==EXIT_STA)
  {
	  u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);
	  u8g2_DrawStr(&u8g2,10,60,"GAME");
	  u8g2_DrawStr(&u8g2,10,70,"OUER");
		
		
		if(VIB)
		 app_sta = RUN_STA;
		
		
  }
  else;

  if(app_sta!=RUN_STA)
	  if(QUIT)
	  {
		  menu.mode = PREFACE;
		  app_sta = DETECT_STA;
		  animation_cnt0 = -64;
	  }
	
	
}



/**
  * @brief  app 5,shows environment data
  * @param  data: ui data structure
  * @retval none
  * @date   2022-02-02
  */

void app_env(Data data)
{
	char buf[50] = {0};
	static app_sta_e  app_sta = ENTER_STA;


    //entrance animation
    static int8_t  animation_cnt0 = -60; //initialize the string position
	static int8_t  animation_cnt1 = 130;
	static int8_t  animation_cnt2 = -10;

	static uint8_t animation_delay0;     //determine the string movement speed
	static uint8_t animation_delay1;
	static uint8_t animation_delay2;

	static uint8_t seq = 0;               //appear sequence   seq=0/1 (the first/second string shows)



	if(QUIT)
	{
		seq = 0;
	    animation_cnt0 = -60;
	    animation_cnt1 = 150;
	    animation_cnt2 = -10;


		menu.mode = PREFACE;
		app_sta = ENTER_STA;
	}


//	if(app_sta==ENTER_STA)
//	{
//		if(seq==0)
//		{
//			if(++animation_delay0==1)
//			{
//				animation_delay0 = 0;
//				animation_cnt0 += 2;
//				if(animation_cnt0==4)
//				{
//					seq = 1;
//			    }
//
//			}
//		}
//
//	    else if(seq==1)
//		{
//			if(++animation_delay1==2)
//			{
//				animation_delay1 = 0;
//				if(--animation_cnt1==120)
//				{
//					seq = 2;
//				}
//
//			}
//
//		}
//
//	    else if(seq==2)
//		{
//			if(++animation_delay2==2)
//			{
//				animation_delay2 = 0;
//				if(++animation_cnt2==9)
//				{
//					seq = 3;
//				}
//
//			}
//		}
//
//
//
//       if(seq==3)
//		{
//            seq = 0;
//            app_sta = RUN_STA;
//		}
//	}



	if(app_sta==RUN_STA)
	{



	}




    u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);


    sprintf(buf,"%.3fm",data.asl);
    u8g2_DrawStr(&u8g2,0,30,buf);
    sprintf(buf,"%.1fHpa  ",data.pressure);
    u8g2_DrawStr(&u8g2,0,50,buf);








}











/**
  * @brief  app preface (RUN_STAing)
  * @param  index: app index
  *            op: operation
  * @retval none 
	* @date   2022-02-02
  */
void app_preface(Data data)
{	
  static uint8_t   delay_cnt;
  static dir_e     dir = MID;    

  static int16_t    x;
  static app_sta_e  app_sta = DETECT_STA;
	
	
  if(app_sta==DETECT_STA)
  {
	if(data.dir==FRWD&& menu.index!= APP_RTC)
	{
		dir = FRWD;
		app_sta = RUN_STA;
	}

	if(data.dir==BKWD && menu.index!= APP_NBR)
	{
	    dir = BKWD;
		app_sta = RUN_STA;
	}

	if(data.dir==MID)
		menu.mode = APP;
  }

	if(app_sta==RUN_STA)
	{
		if(dir==FRWD)
		{
			x++;

			if(x>0)
			  x = 0;
		}

		if(dir==BKWD)
		{
			x--;
		}

		if((x%36)==0)
		{
			if(dir==FRWD)
				--menu.index;

		  if(dir==BKWD)
				++menu.index;
		  app_sta = STBY_STA;
		}
	}

	if(app_sta==STBY_STA)
	{
		if(++delay_cnt==PREFACE_SELECT_DELAY)
		{
			delay_cnt = 0;
			app_sta = DETECT_STA;
		}
	}


	u8g2_SetFont(&u8g2,u8g2_font_open_iconic_app_6x_t);
    u8g2_DrawGlyph(&u8g2,10+2*x,90,69);
    u8g2_DrawGlyph(&u8g2,10+2*x+71,90,72);
    u8g2_DrawGlyph(&u8g2,10+2*x+142,90,65);
    u8g2_DrawGlyph(&u8g2,10+2*x+213,90,70);
    u8g2_DrawGlyph(&u8g2,10+2*x+284,90,64);
    u8g2_DrawGlyph(&u8g2,10+2*x+355,90,67);



	u8g2_SetFont(&u8g2,u8g2_font_sirclivethebold_tr);
	u8g2_DrawStr(&u8g2,18-2*x,120,"RTC");
	u8g2_DrawStr(&u8g2,18-2*x-75,120,"STW");
	u8g2_DrawStr(&u8g2,18-2*x-150,120,"SET");
	u8g2_DrawStr(&u8g2,18-2*x-225,120,"AHRS");
	u8g2_DrawStr(&u8g2,18-2*x-300,120,"GAME");
	u8g2_DrawStr(&u8g2,18-2*x-365,120,"ENU");
}

	


/**
  * @brief  GUI main function
  * @param  data: data structure to be shown
  * @retval none 
  * @date   2022-01-24
  */

void ui_main(Data data)
{
	u8g2_ClearBuffer(&u8g2);

	if(menu.mode==PREFACE)
		menu.preface_excute(data);

	else if(menu.mode==APP)
		menu.app_excute[menu.index](data);

	else;

	u8g2_SendBuffer(&u8g2);
}
