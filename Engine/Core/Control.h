#pragma once

#ifdef GAME_MODE
#define KEY_UP				265		// GLFW_KEY_UP			
#define KEY_DOWN			264		// GLFW_KEY_DOWN		
#define KEY_LEFT			263		// GLFW_KEY_LEFT		
#define KEY_RIGHT			262		// GLFW_KEY_RIGHT	
#define KEY_SPACE			32		// GLFW_KEY_SPACE		
#define L_BUTTON			0		// GLFW_MOUSE_BUTTON_LEFT

#elif ENGINE_MODE
#define KEY_UP				0x01000013
#define KEY_DOWN			0x01000015	
#define KEY_LEFT			0x01000012	
#define KEY_RIGHT			0x01000014
#define KEY_SPACE			0x20	
#define L_BUTTON			0x00000001
#endif

