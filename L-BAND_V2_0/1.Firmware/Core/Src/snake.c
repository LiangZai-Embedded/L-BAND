#include "snake.h"

#define  GAME_SPEED 60   //smaller val, faster speed



//pos:xxxx xxxx  [7:4]x  [3:0]y

#define xy2pos(x,y)     ((x<<4)|y)
#define pos2xy(pos,x,y) do \
	                      {     \
	                         x = pos>>4; \
	                         y = pos&0xF; \
                        }while(0); \
	                         

typedef enum
{
	SNAKE_HIT = 0,
	SNAKE_GROW,
	SNAKE_HOLD
}snake_sta_e;

typedef enum
{
	GAME_RUN = 0,
	GAME_OVER,
	GAME_PLUSE
}game_sta_e;

typedef struct SnakeNode
{
	uint8_t pos;
	struct SnakeNode *next,*prev;
}SNAKELIST;

SNAKELIST snake_head_node;

/**
  * @brief  add a node to the tail of the snake list
  * @param  head: head node of the snake list
  *            x: game map postion x
  *            y: game map postion y
  * @retval none 
	* @date   2022-02-13
  */
void game_snake_add_node(SNAKELIST* head,uint8_t x,uint8_t y) 
{
	SNAKELIST* node_new = (SNAKELIST*)malloc(sizeof(SNAKELIST));
	SNAKELIST* find;
	
	node_new->pos = xy2pos(x,y);
	node_new->next = NULL;
	
	for(find = head;find->next!=NULL;find = find->next){}
	
	find->next = node_new;
	node_new->prev = find; 
	head->prev = node_new;
}


	
/**
  * @brief  creat a snake
  * @param  head: head node of the snake list
  * @retval none 
	* @date   2022-02-13
  */
void game_creat_new_snake(SNAKELIST* head) 
{
	head->pos = xy2pos(5,9);
	
	head->prev = NULL;
	head->next = NULL;
	
	game_snake_add_node(&snake_head_node,5,10);
	game_snake_add_node(&snake_head_node,5,11);


}

/**
  * @brief snake movement
  * @param head: head node of the snake list
  *        dir: movement direction
  * @retval snake status: SNAKE_HIT,SNAKE_GROW,SNAKE_HOLD
	* @date   2022-02-13
  */
snake_sta_e game_snake_move(SNAKELIST* head,dir_e dir,uint8_t food_x,uint8_t food_y)
{
	//snake_sta_e ret = SNAKE_HOLD;
	SNAKELIST* find;
  uint8_t head_x,head_y;
	
	
	
	find = head->prev;
	while(find != head)
	{
		find->pos = find->prev->pos; 
		find = find->prev;
	}
	
	pos2xy(head->pos,head_x,head_y);
	switch(dir)
	{
		case FRWD:  head_y--; break;
		case BKWD:  head_y++; break;
		case LEFT:  head_x--; break;
		case RIGHT: head_x++; break;
		
		default:               break;
	}
		head->pos = xy2pos(head_x,head_y);

	//hit the edge of the display
	if(head_x >GAME_MAP_WIDTH  || head_y >GAME_MAP_HEIGHT )
	return SNAKE_HIT;
		
  //bite itself
	for(find = head->next;find->next!=NULL;find = find->next)
  {
		
		if(head->pos==find->pos)
		
			return SNAKE_HIT;
		
	}
		
	
	//eat food and grow
	if(head_x==food_x && head_y==food_y)
	{
		game_snake_add_node(&snake_head_node,food_x,food_y);
    return SNAKE_GROW;
	}
	
	

	return SNAKE_HOLD;
	
}

/**
  * @brief  free the allowcated memory
  * @param  head: link list head ptr
  * @retval none 
	* @date   2022-02-13
  */
void game_free_snake(SNAKELIST* head) 
{
	SNAKELIST* tail = head->prev;

	while(tail != head)
	{
		tail = tail->prev;
		free(tail->next);
	}
	
}
  

/**
  * @brief  show the snake on the game map
  * @param  head: link list head ptr
  *         food_x,food_y: the food postion on the game map 
  *         which should be transfered to the postion of the display   
	* @retval none
  * @date   2022-02-13
  */
void game_show_snake(SNAKELIST* head,uint8_t food_x,uint8_t food_y)
{
	SNAKELIST* find = head;
	uint8_t X,Y;
  uint8_t find_x,find_y;
	
	u8g2_SetFont(&u8g2,u8g2_font_open_iconic_play_1x_t);

	while(find!=NULL)
	{
		pos2xy(find->pos,find_x,find_y);
		X = 7 * (find_x);
		Y = 7 * ((find_y) + 1);
    
		u8g2_DrawGlyph(&u8g2,X,Y,75);
		
		find = find->next;
	}
	
	X = 7 * food_x;
	Y = 7 * (food_y + 1);
	
	u8g2_DrawGlyph(&u8g2,X,Y,75);
	
}


/**
  * @brief  creat food
  * @param  head: link list head ptr
  *         food_x,food_y: the food postion ptr         
  * @retval none
	* @date   2022-02-13
  */
void game_creat_food(SNAKELIST* head,uint8_t* food_x,uint8_t* food_y)
{	
	uint32_t seed;	
  SNAKELIST* find = head;
	uint8_t find_x,find_y;

	seed = __HAL_TIM_GET_COUNTER(&INTERRUPT_TIM_HANDLE);
	srand(seed);
  *food_x = rand()%GAME_MAP_WIDTH;      
	*food_y = rand()%GAME_MAP_HEIGHT;
	
	for(;;)
	{
		pos2xy(find->pos,find_x,find_y);
		
		if(find_x==*food_x && find_y==*food_y)  
    {
			seed = __HAL_TIM_GET_COUNTER(&INTERRUPT_TIM_HANDLE);
      srand(seed);
      *food_x = rand()%GAME_MAP_WIDTH;      
	    *food_y = rand()%GAME_MAP_HEIGHT;
      find = head; 
		}
		
	  else
		{
		  find = find->next;
		}
		
		if(find==NULL)
			break;
			
	}
	
	
}


/**
  * @brief  get snake movement dir from sensor dir
  * @param  sensor_dir: senor direction
  * @retval snake movement dir 
	* @date   2022-02-13
  */
dir_e game_get_control_dir(dir_e sensor_dir)
{
	static dir_e ret_dir = FRWD;
	
	if(sensor_dir!=MID)   
	{
		if(ret_dir==FRWD)
		{
			if(sensor_dir!=BKWD)
				 ret_dir = sensor_dir;
		}
		
		else if(ret_dir==BKWD)
		{
			if(sensor_dir!=FRWD)
				ret_dir = sensor_dir;
		}
		
		else if(ret_dir==LEFT)
		{
			if(sensor_dir!=RIGHT)
				ret_dir = sensor_dir;
		}
		
		else if(ret_dir==RIGHT)
		{
			if(sensor_dir!=LEFT)
				ret_dir = sensor_dir;
		}
		else;
	}
	
	return ret_dir;
		  
}


/**
  * @brief  game excute
  * @param  head: link list head ptr
  *         pts:  game scores ptr
  * @retval game status which can be GAME_PLUSE,GAME_RUN,GAME_OVER
	* @date   2022-02-13
  */
ErrorStatus game_excute(dir_e dir,uint8_t* pts)
{
	
	static uint16_t     delay_cnt;
	static dir_e       game_dir = FRWD;
	static snake_sta_e snake_sta = SNAKE_HOLD;	
	static game_sta_e  game_sta = GAME_PLUSE;
  static uint8_t food_x,food_y;
	
	if(game_sta==GAME_PLUSE)
	{
		game_creat_new_snake(&snake_head_node);
	  game_creat_food(&snake_head_node,&food_x,&food_y);
    
    game_sta = GAME_RUN;
	}
	
	
	if(game_sta==GAME_RUN)
	{
		if(++delay_cnt==GAME_SPEED)
    {
		  delay_cnt = 0;
		  
      game_dir = game_get_control_dir(dir);
			

		  snake_sta = game_snake_move(&snake_head_node,game_dir,food_x,food_y);
	    
		  if(snake_sta==SNAKE_GROW)
		  {
		  	game_creat_food(&snake_head_node,&food_x,&food_y);
		    (*pts)++;
		  }

			if(snake_sta==SNAKE_HIT) 
			{
				game_sta = GAME_OVER;
			  *pts = 0;
			}
		} 
			
	
    game_show_snake(&snake_head_node,food_x,food_y);
		

	}
	
	if(game_sta==GAME_OVER)
  {
		game_free_snake(&snake_head_node);
    game_sta = GAME_PLUSE;
		game_get_control_dir(FRWD);//rush the static value into the function
		
		return ERROR;
	}

  return SUCCESS;
  
}





