/*+++
* KernelLanguaje.h
+++*/

#include <efi.h>
#include <efilib.h>

// S-SUN kernel Algoritm
#ifndef _KERNEL_LANG_
#define _KERNEL_LANG_

typedef struct {
	CHAR16*
		StandartEnglishOriginal;
	CHAR16*
		StandartSpanish;
	CHAR16*
		StandartFrances;
} Translation;

Translation KernelCallPrefix_TRANSL = { L"* **KERNEL_CALL -> -> : ", L"*** llamada del kernel -> -> :", L"*** KERNEL_CALL -> -> :"  };
Translation SSUNEDITOR_UPPER_BAR_TRANSL = { L"S-SUN editor <ESC = exit> <F1 = see the survival guide> <F5 = run as S++>", L"editor de S-SUN <ESC = salir> <F1 = ver la guia de ayuda> <F5 = ejecutar como un S++>", L"Editeur S-SUN <ESC = quitter> <F1 = voir le guide de survie> <F5 = executer en tant que S++>" };
Translation SSUNEDITOR_LOWER_BAR_TRANSL = { L"<up and down = scroll> <F2 = exit and save state>", L"<arriba y abajo = desplazarse> <F2 = minimizar>", L"<haut et bas = défilement> <F2 = quitter et enregistrer l'état>" };
Translation GAMES_BUTTON_TRANSL = { L"Games", L"Juegos", L"Jeux" };
Translation THEMES_BUTTON_TRANSL = { L"Themes", L"Temas", L"Themes" };
Translation EXITMESSAGE_TRANS = { L"\nApplication closed press any key to continue...", L"\nAplicacion cerrada presione cualquier tecla para continuar...", L"\nApplication fermee appuyez sur n importe quelle touche pour continuer..." };
Translation LANGS_BUTTON_TRANSL = { L"Languaje", L"Idioma", L"Langue" };
Translation GETTIME_BUTTON_TRANSL = { L"Get the time", L"Ver la hora", L"Obtenez l heure" };
Translation GETDATE_BUTTON_TRANSL = { L"Get the date", L"Ver la fecha", L"Obtenir la date" };
Translation TEXTEDITOR_BUTTON_TRANSL = { L"Text Editor", L"Bloc de notas", L"Editeur de texte" };
Translation RESTART_BUTTON_TRANSL = { L"Restart", L"Reiniciar", L"Redemarrage" };
Translation OFF_BUTTON_TRANSL = { L"Shutdown", L"Apagar", L"Fermer" };
Translation CONSOLE_BUTTON_TRANSL = { L"Console", L"Terminal", L"Console" };
Translation SSUNEDSECTION1_TRANSL = { L"S - SUN editor survival guide\n\n", L"guia de ayuda de el editor\n\n", L"Guide de survie de l editeur S-SUN\n\n" };
Translation SSUNEDSECTION1TEXT_TRANSL = { L"    \x4 S-SUN editor is a powerful tool for making documents\n    or programs.\n\n", L"    \x4 el editor de S-SUN es una poderosa herramienta para hacer documentos\n    o programas.\n\n", L"    \x4 L editeur S-SUN est un outil puissant pour créer des documents\n    ou des programmes.\n\n" };
Translation SSUNEDSECTION2_TRANSL = { L"How To Use\n\n", L"Como usarlo\n\n", L"Comment utiliser\n\n" };
Translation SSUNEDSECTION2TEXT_TRANSL = { L"    \x4 use it as another editor, to make programs press F5\n\n    \x4 NOTE: you can't save your file\n    so don't make big programs", L"    \x4 usalo como qualquier otro editor, para hacer programas presione F5\n\n    \x4 NOTA: no puedes guadar los archivos\n    asi que, no hagas programas grandes", L"    \x4 utilisez-le comme un autre editeur, pour creer des programmes, appuyez sur F5\n\n    \x4 REMARQUE : vous ne pouvez pas enregistrer votre fichier\n donc ne creez pas de gros programmes" };

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