/*+++
* KernelLanguaje.h
+++*/

#include "../licensesafety.h"
#include <efi.h>
#include <efilib.h>

// S-SUN kernel multi languaje
#ifndef _KERNEL_LANG_
#define _KERNEL_LANG_

CHAR16* languajecu;

typedef struct {
	CHAR16*
		StandartEnglishOriginal;
	CHAR16*
		StandartSpanish;
	CHAR16*
		StandartFrances;
} Translation;

Translation KernelCallPrefix_TRANSL = { L"* **KERNEL_CALL -> -> : ", L"*** LLAMADA DEL KERNEL -> -> :", L"*** KERNEL_CALL -> -> :"  };
Translation SSUNEDITOR_UPPER_BAR_TRANSL = { L"S-SUN editor <ESC = exit> <F1 = see the survival guide> <F5 = run as S++>", L"editor de S-SUN <ESC = salir> <F1 = ver la guia de ayuda> <F5 = ejecutar como un S++>", L"Editeur S-SUN <ESC = quitter> <F1 = voir le guide de survie> <F5 = executer en tant que S++>" };
Translation SSUNEDITOR_LOWER_BAR_TRANSL = { L"<up and down = scroll> <F2 = exit and save state>", L"<arriba y abajo = desplazarse> <F2 = minimizar>", L"<haut et bas = d�filement> <F2 = quitter et enregistrer l'�tat>" };
Translation GAMES_BUTTON_TRANSL = { L"Games", L"Juegos", L"Jeux" };
Translation TEST_BUTTON_TRANSL = { L"Tests", L"Pruebas", L"tests" };
Translation THEMES_BUTTON_TRANSL = { L"Themes", L"Temas", L"Themes" };
Translation EXITMESSAGE_TRANS = { L"\nApplication closed press any key to continue...", L"\nAplicacion cerrada presione cualquier tecla para continuar...", L"\nApplication fermee appuyez sur n importe quelle touche pour continuer..." };
Translation LANGS_BUTTON_TRANSL = { L"Langs", L"Idioma", L"Langue" };
Translation SPLASH_OP2_TRANSL = { L". worng boot option?", L". sistema equivocado?", L". option de demarrage usee ?" };
Translation SPLASH_OP1_TRANSL = { L". skip this screen", L". saltar al arranque", L". ignorer cet ecran" };
Translation GENERIC_TEXT1_TRANSL = { L"Please Wait...", L"Por favor espere...", L"S il vous plait, attendez..." };
Translation SPLASH_TEXT1_TRANSL = { L"booting in:              ", L"Iniciando en:            ", L"demarrage dans :         " };

Translation GENERIC2_TEXT_TRANSL = { L"typ� ", L"escriba ", L"type" };

Translation GENERIC1_TEXT_TRANSL = { L"Welcome to ", L"Bienvenido a la ", L"Welcome to " };
Translation CONSOLE_INSTR1_TRANSL = { L"if is your first time here \nfollow this steps:\n\n", L"si es tu primera vez aqui \nsiga estos pasos para comenzar:\n\n", L"si c est votre premiere fois ici\nsuivez ces etapes�:\n\n" };
Translation CONSOLE_INSTR2_TRANSL = { L"for a list of begimmer commands\n", L"para ver una lista de comandos que todos deberiamos saber\n", L"pour une liste des commandes begimmer\n" };
Translation CONSOLE_INSTR3_TRANSL = { L"for exit from the ", L"para salir de la ", L"pour sortir de la " };
Translation WARNING_NOT_T_TRANSL = { L"", L"fijese que solo hay traduccion para el ingles", L"Notez qu il n y a qu une traduction en anglais" };

Translation CMN_BUTTON_TRANSL = { L"Mod Programs", L"Programas del creador de la modificacion", L"Mod Programs" };
Translation GETTIME_BUTTON_TRANSL = { L"Get the time", L"Ver la hora", L"Obtenez l heure" };
Translation GETDATE_BUTTON_TRANSL = { L"Get the date", L"Ver la fecha", L"Obtenir la date" };
Translation TEXTEDITOR_BUTTON_TRANSL = { L"Text Editor", L"Bloc de notas", L"Editeur de texte" };
Translation RESTART_BUTTON_TRANSL = { L"Restart", L"Reiniciar", L"Redemarrage" };
Translation OFF_BUTTON_TRANSL = { L"Shutdown", L"Apagar", L"Fermer" };
Translation CONSOLE_BUTTON_TRANSL = { L"Console", L"Terminal", L"Console" };
Translation CALC_BUTTON_TRANSL = { L"Calculator", L"Calculadora", L"Calculator" };
Translation SSUNEDSECTION1_TRANSL = { L"S - SUN editor survival guide\n\n", L"guia de ayuda de el editor\n\n", L"Guide de survie de l editeur S-SUN\n\n" };
Translation SSUNEDSECTION1TEXT_TRANSL = { L"    \x2a4 S-SUN editor is a powerful tool for making documents\n    or programs.\n\n", L"    \x2a4 el editor de S-SUN es una poderosa herramienta para hacer documentos\n    o programas.\n\n", L"    \x2a4 L editeur S-SUN est un outil puissant pour cr�er des documents\n    ou des programmes.\n\n" };
Translation SSUNEDSECTION2_TRANSL = { L"How To Use\n\n", L"Como usarlo\n\n", L"Comment utiliser\n\n" };
Translation SSUNEDSECTION2TEXT_TRANSL = { L"    \x2a4 use it as another editor, to make programs press F5\n\n    \x2a4 NOTE: you can't save your file\n    so don't make big programs", L"    \x2a4 usalo como qualquier otro editor, para hacer programas presione F5\n\n    \x2a4 NOTA: no puedes guadar los archivos\n    asi que, no hagas programas grandes", L"    \x2a4 utilisez-le comme un autre editeur, pour creer des programmes, appuyez sur F5\n\n    \x2a4 REMARQUE�: vous ne pouvez pas enregistrer votre fichier\n donc ne creez pas de gros programmes" };
Translation RESETCONFIRM_TRANS = { L"Do you want to restart?", L"Seguro que deseas reiniciar el sistema?", L"Voulez-vous redemarrer ?" };
Translation SHUTDOWNCONFIRM_TRANS = { L"Do you want to shutdown?", L"Seguro que deseas apagar la PC?", L"Voulez-vous arreter ?" };
Translation HELP_BUTTON_TRANS = { L"Help", L"Ayuda", L"Aide" };
Translation ARTIST_BUTTON_TRANS = { L"Artist", L"Artista", L"Artist" };
Translation HELP_TEXT_TRANS = { 
	L"section::S-SUN Survival Guide:\n\n\n    \x2a4 Welcome to S-SUN a operating system \n    maded by ErickCraftStudios\n\nsection::Shortcuts:\n\n\n    \x2a4 F12:\n    this key is for acces to the misc \n    characters this is a list of a combinations:\n    1=\x2a1\n    2=\x2a2\n    3=\x2a3\n    4=\x2a4\n    5=\x2a5\n    6=\x2a6", 
	L"section::guia de ayuda de S-SUN:\n\n\n    \x2a4 Bienvenido a S-SUN , un sistema operativo hecho por ErickCraftStudios\n\nsection::Accesos Rapidos:\n\n\n    \x2a4 F12:\n    esta tecla es usada para acceder \n    a los caracteres especiales, esta \n    es una lista de las combinaciones:\n    1=\x2a1\n    2=\x2a2\n    3=\x2a3\n    4=\x2a4\n    5=\x2a5\n    6=\x2a6",
	L"Aide"
};
Translation BOOTINGUP_MSG_TRANS = {
	L"Please wait while S-SUN is Booting Up...",
	L"Por favor espere mientras lo bueno empieze...",
	L"Veuillez patienter pendant le d�marrage de S-SUN..."
};
Translation LOGOUT_MSG_TRANS = {
	L"Please wait while S-SUN logins out...",
	L"Por favor espere se cierre la session...",
	L"Veuillez patienter pendant que S-SUN se d�connecte..."
};
Translation CRTMEM_LOG_MSG_TRANS = {
	L"Status on creating the MEM",
	L"Resultado al crear el registro",
	L"Etat de la cr�ation du MEM"
};
Translation OEM_NOT_STABLE_MSG_TRANS = {
	L"Warnning : you are using a in devlopment version",
	L"Advertencia : esta usando una version sin terminar",
	L"Attention : vous utilisez une version en developpement"
};
Translation IS_STABLE_VER_TRANS = {
	L"is a oem stable version or a comercial version",
	L"es una version comercial",
	L"est-ce une version stable OEM ou une version commerciale"
};
Translation EXIT_EDITOR_TEXT = { L"Do you want to save?", L"Quieres guardar el documento?", L"Voulez-vous economiser ?" };
Translation SCRIPT_EXIT_EDITOR_TEXT = { L"Do you want to exit from the script?", L"Quieres detener el mini programa?", L"Voulez-vous quitter le script ?" };
Translation SETUPCONF_MSG_TRANS = { L"Do you want to install S-SUN in: ", L"Seguro que desea instalar S-SUN en: ", L"Voulez-vous installer S-SUN dans :" };

CHAR16*
TranslateWorck
(
	Translation* Transalatedictionarie,
	CHAR16* Language
)
{
	if (StrCmp(Language, L"en") == 0) {
		return Transalatedictionarie->StandartEnglishOriginal;
	} 
	else if (StrCmp(Language, L"fr") == 0) {
		return Transalatedictionarie->StandartFrances;
	}
	else if (StrCmp(Language, L"es") == 0) {
		return Transalatedictionarie->StandartSpanish;
	}
	else
	{
		return Transalatedictionarie->StandartEnglishOriginal;
	}
}
#endif // !_KERNEL_LANG_