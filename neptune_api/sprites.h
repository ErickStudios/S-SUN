/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

	sprites.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

	a easy form to make games

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#include "../kernel/licensesafety.h"

/*
the developers want a form to develop games faster

this is the solution library
*/
// i called it "BlynTool"

// this have much bugs so , GOOD LUCK TRYING TO MAKING GAMES HERE

#if 0
#ifndef BlynTool_H
#define BlynTool_H

#include "KernelTextMode.h"

#define MAX_SPRITES_NUMBER	10
#define MAX_EVENTS			10

typedef UINTN BlynTool_COLOR;

PIXELCOL ColorToPixel(BlynTool_COLOR Color, PIXELCOL BG)
{
	switch (Color)
	{
	case 0:  return BG;
	case 1:  return black;
	case 2:  return lightblack;
	case 3:  return gray;
	case 4:  return lightgray;
	case 5:  return darkgray;
	case 6:  return white;
	case 7:  return red;
	case 8:  return darkred;
	case 9:  return brred;
	case 10: return orange;
	case 11: return darkorange;
	case 12: return brorange;
	case 13: return yellow;
	case 14: return darkyellow;
	case 15: return bryellow;
	case 16: return green;
	case 17: return darkgreen;
	case 18: return brgreen;
	case 19: return teal;
	case 20: return darkteal;
	case 21: return brteal;
	case 22: return cyan;
	case 23: return darkcyan;
	case 24: return brcyan;
	case 25: return blue;
	case 26: return darkblue;
	case 27: return brblue;
	default: return BG;
	}
}

typedef struct {
	// position changes
	BOOLEAN movx;
	INTN x;

	BOOLEAN movy;
	INTN y;

	// costume changes
	BOOLEAN change_costume;
	UINTN costume;

	BOOLEAN ActiveChange;

	UINTN SpriteToChange;
} BlynTool_SPRITE_CHANGE;

typedef struct {
	// key class
	BOOLEAN OnKeyActive;
	EFI_INPUT_KEY KeyToActive;

	BOOLEAN OnUpdateScreenAndKeyReadedWhitoutWaitToTheKey;		// this complements to OnUpdateScreen

	// special on update screen event
	BOOLEAN OnUpdateScreen;
	BOOLEAN OnGameLoop;

	// sprite change events
	BOOLEAN ChangeSprites;
	BlynTool_SPRITE_CHANGE SpritesToChange[MAX_SPRITES_NUMBER];

	BOOLEAN EventEnabled;

	// miscelianus
	BOOLEAN ExitOfGameOnActive;

	// system
	BOOLEAN MakeAsysActionOnActive;
	CHAR16* SPLUSPLUS_COMMAND;
}
BlynTool_EVENT
;

typedef struct {
	BOOLEAN VoidLine;

	// text
	BOOLEAN IsTextLabel;

	// position
	INTN x;
	INTN y;
	BOOLEAN InvertX;							// the x position if 0 is now on the screen right side

	BOOLEAN Static;								 // this blocks the motion things

	// motion
	BOOLEAN MovX;								// this mechanic is used on fisher game
	INTN SpeedX;

	BOOLEAN MovY;								// this mechanic is used on fisher game
	INTN SpeedY;

	// the sprite
	BlynTool_COLOR Color;
	CHAR16 Costume[5][512];						// costume , trick: L'\x2763' = next line and go to the x start msg (the sprite string)
	CHAR16* Text;								// the text

	BOOLEAN ShowingSprite;						// if the sprite is not hidden

	UINTN CurrentCostume;						// the current costume
}
BlynTool_SPRITE
;

typedef struct {
	BlynTool_SPRITE Sprites[MAX_SPRITES_NUMBER];	// the game sprites

	BlynTool_EVENT Events[MAX_EVENTS];				// the game sprites

	BlynTool_COLOR Background;

	UINTN UpdateScreenDelay;

}
BlynTool_BASED_GAME
;

/*
BlynTool_RunGame

Summary:
	run a game maded with BlynTool
*/
EFI_STATUS
BlynTool_RunGame
(
	BlynTool_BASED_GAME* Game
)
{
	// input variables
	UINTN Event;
	EFI_INPUT_KEY Key;
	BlynTool_BASED_GAME* InRunningGame = Game;				// this is the editable in memory game

	while (
		TRUE
		)
	{
		Print(L"Game.Background antes de copiar: %d\n", Game->Background);

		if (
			InRunningGame->Events != NULL
			) {
			// the loop for the OnGameLoop events
			for (
				size_t i = 0;
				i < MAX_EVENTS;
				i++
				)
			{
				BlynTool_EVENT Event = InRunningGame->Events[i];
				if (
					Event.OnGameLoop
					)
				{
					if (
						Event.ChangeSprites // if change sprites
						)
					{
						// change sprites
						for (
							size_t j = 0;
							j < MAX_SPRITES_NUMBER;
							j++
							)
						{
							BlynTool_SPRITE_CHANGE SpriteAction = Event.SpritesToChange[j];
							if (
								SpriteAction.ActiveChange
								)
							{
								if (
									SpriteAction.movx
									)
									InRunningGame->Sprites[SpriteAction.SpriteToChange].x += SpriteAction.x
									;

								if (
									SpriteAction.movy
									)
									InRunningGame->Sprites[SpriteAction.SpriteToChange].y += SpriteAction.y
									;

								if (
									SpriteAction.change_costume
									)
									InRunningGame->Sprites[SpriteAction.SpriteToChange].CurrentCostume = SpriteAction.costume
									;
							}
						}
					}
				}
			}
		}

		Print(L"Background value: %d\n", InRunningGame->Background);
		PIXELCOL Color = ColorToPixel(InRunningGame->Background, black);
		SetScreenAtribute(1, Color);
		ClearScreen();

		if (
			InRunningGame->Sprites != NULL
			) {
			// the sprite render
			for (
				size_t SpriteRender = 0;
				SpriteRender < MAX_SPRITES_NUMBER;
				SpriteRender++
				)
			{
				BlynTool_SPRITE Sprite = InRunningGame->Sprites[SpriteRender];

				if (
					Sprite.ShowingSprite
					) {
					gotoxy(
						Sprite.InvertX ? ((GET_MAX_COL)-Sprite.x) : (Sprite.x),
						Sprite.y
					);
					if (
						Sprite.IsTextLabel
						) {
						SetScreenAtribute(0, ColorToPixel(Sprite.Color, Color));
						printcu(Sprite.Text);
					}
					else {
						SetScreenAtribute(0, ColorToPixel(Sprite.Color, Color));
						printcu(Sprite.Costume[Sprite.CurrentCostume]);
					}
				}
			}
		}
		DrawScreen();

		gBS->Stall(InRunningGame->UpdateScreenDelay);
		gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

		if (
			InRunningGame->Events != NULL
			) {
			// the loop for the OnUpdateScreen events
			for (
				size_t i = 0;
				i < MAX_EVENTS;
				i++
				)
			{
				BlynTool_EVENT Event = InRunningGame->Events[i];
				if (
					Event.OnUpdateScreen
					)
				{
					if (
						Event.OnUpdateScreenAndKeyReadedWhitoutWaitToTheKey
						)
					{
						if (
							Event.KeyToActive.ScanCode == Key.ScanCode &&
							Event.KeyToActive.UnicodeChar == Key.UnicodeChar
							)
						{
						}
						else
						{
							continue;
						}
					}
					if (
						Event.ExitOfGameOnActive
						)
					{
						return 0;
					}
					if (
						Event.ChangeSprites // if change sprites
						)
					{
						// change sprites
						for (
							size_t j = 0;
							j < MAX_SPRITES_NUMBER;
							j++
							)
						{
							BlynTool_SPRITE_CHANGE SpriteAction = Event.SpritesToChange[j];
							if (
								SpriteAction.ActiveChange
								)
							{
								if (
									SpriteAction.movx
									)
									InRunningGame->Sprites[SpriteAction.SpriteToChange].x += SpriteAction.x
									;

								if (
									SpriteAction.movy
									)
									InRunningGame->Sprites[SpriteAction.SpriteToChange].y += SpriteAction.y
									;

								if (
									SpriteAction.change_costume
									)
									InRunningGame->Sprites[SpriteAction.SpriteToChange].CurrentCostume = SpriteAction.costume
									;
							}
						}
					}
				}
			}
		}
	}
}
#endif
#endif