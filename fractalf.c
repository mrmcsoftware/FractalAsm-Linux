/* Sample assembly language program for the x86 CPU by
   Mark Craig (mandelbrot and julia fractals) (Linux)
   https://www.youtube.com/MrMcSoftware */

/* I chose to use assembly and C mix because I didn't want to code the Linux
   GUI stuff in assembly.  My original assembly language fractal program was
   written for a CPU I designed.  That version was all assembly and was the
   basis for this x86 version. */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <Xm/MessageB.h>
#include <Xm/FileSB.h>
#include <Xm/DialogS.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/TextF.h>
#include <Xm/PushB.h>
#include <Xm/ToggleBG.h>
#include <Xm/SeparatoG.h>
#include <X11/Xos.h>		/* Not needed if include string.h */
//#include <X11/Xatom.h>	/* Not needed */
#ifdef HAVE_SDL2
#include <SDL2/SDL.h>
#endif
#ifndef NO_XPM
#include <X11/xpm.h>
#include "myicon.xpm"
#endif

#define MANDELBROT 0
#define JULIA 1

typedef struct Points		/* mimic Windows POINTS type */
	{
	int x, y;
	} POINTS;

	/* Linux GUI variables */

GC gc;						/* XWindow Graphic Context */
Display *dpy;				/* Connection to the X server */
Window win;					/* Window handle */
XImage *ximage = NULL;		/* XWindow bitmap image structure */
Atom wmDeleteMessage;		/* Message sent when user clicks "x" close button */
int width, height;			/* Window client area dimensions */
POINTS pt0, pts, pte;		/* zoom: old point, start point, end point */
Widget text_w_1, text_w_2;	/* Motif textfield widget handles for fractal type*/
Pixmap icon;				/* Program icon */
char filename[260];

	/* Variables used in assembly part */

	/* some of these variables are vestiges of my original assembly language
	   fractal program for my CPU design which this x86 version is based on.
	   That CPU design was simpler than x86, so these vars. were necessary. */

unsigned char im[512*512*4];	/* Image buffer */
float aedge1 = -2.0, bedge1 = -1.25, xdelta1 = 3.0, ydelta1 = 2.5;
float aedge2 = -1.061955193, bedge2 = -.264645193;
float xdelta2 = .000230386, ydelta2 = .000230386;
float aedge3 = -1.2125216, bedge3 = -.3355226;
float xdelta3 = .1620652, ydelta3 = .1620652;
float aedge4 = -1.5, bedge4 = -1.4, xdelta4 = 3.0, ydelta4 = 2.8;
float aedge5 = 0.33285461273437479100, bedge5 = 0.57068114093749985900;
float xdelta5 = 0.1395625000000000000e-04, ydelta5 = 0.1395625000000000000e-04;
float a1 = -.8, b1 = .156, a2 = -.742, b2 = .1, a3 = -.4, b3 = .6;
float a4 = .285, b4 = .01, a5 = 0.0, b5 = -.8, a6 = -.835, b6 = -.2321;
float a7 = -.1, b7 = .651, a8 = .3, b8 = 0.0, a = -.8, b = .156;
float xres1 = 512.0, yres1 = 512.0, xres = 512.0, yres = 512.0;
float xres2 = 256.0, yres2 = 256.0;
float one = 1.0, two = 2.0, converge = 4.0, popped;
float m, n, xgap, ygap, ac, bc, az, bz, sizev, temp, tempb;
float aedge = -2.0, bedge = -1.25, xdelta = 3.0, ydelta = 2.5;
int numpixels = 512*512*4, numpixels1 = 512*512*4, numpixels2 = 512*256*4;
int off = 0, ncols = 255, count, limit = 1023;
int pixel, cyinc = 2, cydir = 1;
int resadd = 0, resadd1 = 0, resadd2 = 256*4;
int imaged[512*512];		/* "count" buffer used for color cycling */

	/* Rainbow colortable generated by HSV to RGB conversion (hue: 0 to 360) */

unsigned char Colors[256*3] = {
	0,0,0,
	255,6,0,
	255,12,0,
	255,18,0,
	255,24,0,
	255,30,0,
	255,36,0,
	255,42,0,
	255,48,0,
	255,54,0,
	255,60,0,
	255,66,0,
	255,72,0,
	255,78,0,
	255,84,0,
	255,90,0,
	255,96,0,
	255,102,0,
	255,108,0,
	255,114,0,
	255,120,0,
	255,126,0,
	255,132,0,
	255,138,0,
	255,144,0,
	255,150,0,
	255,156,0,
	255,162,0,
	255,168,0,
	255,174,0,
	255,180,0,
	255,186,0,
	255,192,0,
	255,198,0,
	255,204,0,
	255,210,0,
	255,216,0,
	255,222,0,
	255,228,0,
	255,234,0,
	255,240,0,
	255,246,0,
	255,252,0,
	252,255,0,
	246,255,0,
	239,255,0,
	233,255,0,
	227,255,0,
	221,255,0,
	215,255,0,
	209,255,0,
	203,255,0,
	197,255,0,
	191,255,0,
	185,255,0,
	179,255,0,
	173,255,0,
	167,255,0,
	161,255,0,
	155,255,0,
	149,255,0,
	143,255,0,
	137,255,0,
	131,255,0,
	125,255,0,
	119,255,0,
	113,255,0,
	107,255,0,
	101,255,0,
	95,255,0,
	89,255,0,
	83,255,0,
	77,255,0,
	71,255,0,
	65,255,0,
	59,255,0,
	53,255,0,
	47,255,0,
	41,255,0,
	35,255,0,
	29,255,0,
	23,255,0,
	17,255,0,
	11,255,0,
	5,255,0,
	0,255,0,
	0,255,6,
	0,255,12,
	0,255,18,
	0,255,24,
	0,255,30,
	0,255,36,
	0,255,42,
	0,255,48,
	0,255,54,
	0,255,60,
	0,255,66,
	0,255,72,
	0,255,78,
	0,255,84,
	0,255,90,
	0,255,96,
	0,255,102,
	0,255,108,
	0,255,114,
	0,255,120,
	0,255,126,
	0,255,132,
	0,255,138,
	0,255,144,
	0,255,150,
	0,255,156,
	0,255,162,
	0,255,168,
	0,255,174,
	0,255,180,
	0,255,186,
	0,255,192,
	0,255,198,
	0,255,204,
	0,255,210,
	0,255,216,
	0,255,222,
	0,255,228,
	0,255,234,
	0,255,240,
	0,255,246,
	0,255,252,
	0,251,255,
	0,245,255,
	0,239,255,
	0,233,255,
	0,227,255,
	0,221,255,
	0,215,255,
	0,209,255,
	0,203,255,
	0,197,255,
	0,191,255,
	0,185,255,
	0,179,255,
	0,173,255,
	0,167,255,
	0,161,255,
	0,155,255,
	0,149,255,
	0,143,255,
	0,137,255,
	0,131,255,
	0,125,255,
	0,119,255,
	0,113,255,
	0,107,255,
	0,101,255,
	0,95,255,
	0,89,255,
	0,83,255,
	0,77,255,
	0,71,255,
	0,65,255,
	0,59,255,
	0,53,255,
	0,47,255,
	0,41,255,
	0,35,255,
	0,29,255,
	0,23,255,
	0,17,255,
	0,11,255,
	0,5,255,
	0,0,255,
	6,0,255,
	12,0,255,
	18,0,255,
	24,0,255,
	30,0,255,
	36,0,255,
	42,0,255,
	48,0,255,
	54,0,255,
	60,0,255,
	66,0,255,
	72,0,255,
	78,0,255,
	84,0,255,
	90,0,255,
	96,0,255,
	102,0,255,
	108,0,255,
	114,0,255,
	120,0,255,
	126,0,255,
	132,0,255,
	138,0,255,
	144,0,255,
	150,0,255,
	156,0,255,
	162,0,255,
	168,0,255,
	174,0,255,
	180,0,255,
	186,0,255,
	192,0,255,
	198,0,255,
	204,0,255,
	210,0,255,
	216,0,255,
	222,0,255,
	228,0,255,
	234,0,255,
	240,0,255,
	246,0,255,
	252,0,255,
	255,0,251,
	255,0,245,
	255,0,239,
	255,0,233,
	255,0,227,
	255,0,221,
	255,0,215,
	255,0,209,
	255,0,203,
	255,0,197,
	255,0,191,
	255,0,185,
	255,0,179,
	255,0,173,
	255,0,167,
	255,0,161,
	255,0,155,
	255,0,149,
	255,0,143,
	255,0,137,
	255,0,131,
	255,0,125,
	255,0,119,
	255,0,113,
	255,0,107,
	255,0,101,
	255,0,95,
	255,0,89,
	255,0,83,
	255,0,77,
	255,0,71,
	255,0,65,
	255,0,59,
	255,0,53,
	255,0,47,
	255,0,41,
	255,0,35,
	255,0,29,
	255,0,23,
	255,0,17,
	255,0,11,
	255,0,5,
	255,0,5};

	/* Variables used in C part */

float aedgeUndo, bedgeUndo, xdeltaUndo, ydeltaUndo;	/* zoom undo vars */
int zoomgetpoint = 0, zoomtype, keepitsquare = 1;		/* zoom vars */
int speed = 0, dspeed = 10, pauseit = 0, cycle = 0;		/* color cycle vars */
int fractaltype = MANDELBROT;

int graphinit(), graphterm(), ProcessEvents();
void SaveImage();

	/* Routines from assembly */

extern void _Mandelbrot(), _Julia(), _Cycle(), Choice0(), Choice1(), Choice2();
extern void Choice3(), Choice4(), Choice5(), Choice6(), Choice7(), Choice8();
extern void Choice9(), Choicea(), Choiceb(), Choicem();

	/* A few routines that call the assembly routines.  BTW, I purposely chose
	   to use basic x86 assembly - no SSE*, no AVX-512, etc. */

void Mandelbrot()
{
_Mandelbrot();
XPutImage(dpy, win, gc, ximage, 0, 0, 0, 0, ximage->width, ximage->height);
}
			
void Julia()
{
_Julia();
XPutImage(dpy, win, gc, ximage, 0, 0, 0, 0, ximage->width, ximage->height);
}
			
	/* Colortable cycling */

void Cycle()
{
_Cycle();
XPutImage(dpy, win, gc, ximage, 0, 0, 0, 0, ximage->width, ximage->height);
}

	/* Linux / Motif stuff */

/* callback function for parameters dialog box */

void paramcallback(Widget widget, XtPointer client_data, XtPointer call_data)
{
XmToggleButtonCallbackStruct *cbs = call_data;
char *text = NULL;
long i;

i = (long)client_data;
if ((i >= 2) && (i <= 8)) { text = XmTextFieldGetString(widget); }
else if ((i < 2) && (cbs->set == XmUNSET)) { return; } // Don't need UnSet event
switch (i)
	{
	case 0: fractaltype = MANDELBROT; // Make Julia params uneditable
		XtVaSetValues(text_w_1, XmNeditable, False, NULL);
		XtVaSetValues(text_w_2, XmNeditable, False, NULL);
		break;
	case 1: fractaltype = JULIA; // Make Julia params editable
		XtVaSetValues(text_w_1, XmNeditable, True, NULL);
		XtVaSetValues(text_w_2, XmNeditable, True, NULL);
		break;
	case 2: sscanf(text, "%f", &aedge); break;
	case 3: sscanf(text, "%f", &bedge); break;
	case 4: sscanf(text, "%f", &xdelta); break;
	case 5: sscanf(text, "%f", &ydelta); break;
	case 6: sscanf(text, "%f", &a); break;
	case 7: sscanf(text, "%f", &b); break;
	case 8: sscanf(text, "%d", &limit); break;
	case 101: if (fractaltype == MANDELBROT) { Mandelbrot(); } else { Julia(); }
	case 100: XtUnmanageChild(XtParent(XtParent(XtParent(widget))));
		// can also do: XtDestroyWidget(XtParent(XtParent(XtParent(widget))));
		break;
	}
if (text) { XtFree(text); }
}

/* callback function for file selector */

void fileselcallback(Widget widget, XtPointer client_data, XtPointer call_data)
{
char *name = (char *)client_data, *str;
XmFileSelectionBoxCallbackStruct *cbs = call_data;

XtUnmanageChild(widget);
if (cbs->reason == XmCR_OK)
	{
	if (!(str = XmStringUnparse(cbs->value, XmFONTLIST_DEFAULT_TAG, XmCHARSET_TEXT, XmCHARSET_TEXT, 0, 0, XmOUTPUT_ALL))) { return; }
	strcpy(name, str);
	XtFree(str);
	if (strcmp(name+strlen(name)-4, ".ppm") !=0 ) { strcat(name, ".ppm"); }
	SaveImage(name);
	}
}

void parameters()
{
Widget dialog, okb, cancelb, rowcol, form, label_w, text_w;
Widget radiobox, rbm, rbj, sep, toplevel;
XtAppContext app;
Arg arg[15];
XmString ok, cancel;
int sargc = 1;
char *sargv[] = {"fractald"};
int n = 0, pos;
long i;
char str[256], *labels[] = { "a edge:", "b edge:", "x delta:", "y delta:", "Julia a:", "Julia b:", "Limit:" };

toplevel = XtVaOpenApplication(&app, "params", NULL, 0, &sargc, sargv, NULL, sessionShellWidgetClass, NULL);
XtSetArg(arg[n], XmNdeleteResponse, XmDESTROY); n++;
dialog = XmCreateDialogShell(toplevel, "Parameters", arg, n); n = 0;
rowcol = XmCreateRowColumn(dialog, "rowcol", NULL, 0);
for (i=0; i<7; i++)
	{
	if (i%2 != 0) { pos = 21; }
	else
		{
		pos = 0;
		n = 0;
		XtSetArg(arg[n], XmNfractionBase, 40); n++;
		XtSetArg(arg[n], XmNnavigationType, XmNONE); n++;
		form = XmCreateForm(rowcol, "form", arg, n);
		}
	n = 0;
	XtSetArg(arg[n], XmNtopAttachment, XmATTACH_FORM); n++;
	XtSetArg(arg[n], XmNbottomAttachment, XmATTACH_FORM); n++;
	XtSetArg(arg[n], XmNleftAttachment, XmATTACH_POSITION); n++;
	XtSetArg(arg[n], XmNleftPosition, pos); n++;
	XtSetArg(arg[n], XmNrightAttachment, XmATTACH_POSITION); n++;
	XtSetArg(arg[n], XmNrightPosition, 5+pos); n++;
	XtSetArg(arg[n], XmNalignment, XmALIGNMENT_END); n++;
	XtSetArg(arg[n], XmNnavigationType, XmNONE); n++;
	label_w = XmCreateLabelGadget(form, labels[i], arg, n);
	XtManageChild(label_w);
	n = 0;
	XtSetArg(arg[n], XmNtraversalOn, True); n++;
	XtSetArg(arg[n], XmNleftAttachment, XmATTACH_POSITION); n++;
	XtSetArg(arg[n], XmNleftPosition, 6+pos); n++;
	XtSetArg(arg[n], XmNrightAttachment, XmATTACH_POSITION); n++;
	XtSetArg(arg[n], XmNrightPosition, 19+pos); n++;
	XtSetArg(arg[n], XmNnavigationType, XmTAB_GROUP); n++;
	if (((i == 4) || (i == 5)) && (fractaltype == MANDELBROT)) { XtSetArg(arg[n], XmNeditable, False); n++; }
	text_w = XmCreateTextField(form, "text_w", arg, n);
	if (i == 4) { text_w_1 = text_w; }
	else if (i == 5) { text_w_2 = text_w; }
	switch (i)
		{
		case 0: sprintf(str,"%.10f", aedge); break;
		case 1: sprintf(str,"%.10f", bedge); break;
		case 2: sprintf(str,"%.10f", xdelta); break;
		case 3: sprintf(str,"%.10f", ydelta); break;
		case 4: sprintf(str,"%.10f", a); break;
		case 5: sprintf(str,"%.10f", b); break;
		case 6: sprintf(str,"%d", limit); break;
		}
	XmTextFieldSetString(text_w, str);
	XtManageChild(text_w);
	XtAddCallback(text_w, XmNvalueChangedCallback, paramcallback, (XtPointer)(i+2));
	XtManageChild(form);
	XtVaSetValues(form, XmNwidth, 400, NULL);
	}
n = 0;
XtSetArg(arg[n], XmNorientation, XmHORIZONTAL); n++;
radiobox = XmCreateRadioBox(rowcol, "radiobox", arg, n);
n = 0;
//XtSetArg(arg[n], XmNindicatorType, XmN_OF_MANY); n++;
XtSetArg(arg[n], XmNindicatorType, XmONE_OF_MANY_ROUND); n++;
XtSetArg(arg[n], XmNselectColor, 0xff0000); n++;
XtSetArg(arg[n], XmNset, fractaltype == MANDELBROT); n++;
XtSetArg(arg[n], XmNnavigationType, XmTAB_GROUP); n++;
//XtSetArg(arg[n], XmNorientation, XmHORIZONTAL); n++;
rbm = XmCreateToggleButtonGadget(radiobox, "Mandelbrot", arg, n);
XtAddCallback(rbm, XmNvalueChangedCallback, paramcallback, (XtPointer)0);
XtManageChild(rbm);
n = 0;
//XtSetArg(arg[n], XmNindicatorType, XmN_OF_MANY); n++;
XtSetArg(arg[n], XmNindicatorType, XmONE_OF_MANY_ROUND); n++;
XtSetArg(arg[n], XmNselectColor, 0xff0000); n++;
XtSetArg(arg[n], XmNset, fractaltype == JULIA); n++;
XtSetArg(arg[n], XmNnavigationType, XmTAB_GROUP); n++;
rbj = XmCreateToggleButtonGadget(radiobox, "Julia", arg, n);
XtAddCallback(rbj, XmNvalueChangedCallback, paramcallback, (XtPointer)1);
XtManageChild(rbj);
XtManageChild(radiobox);
// Can also do:
/*
	rbm = XmStringCreateSimple("Mandelbrot");
	rbj = XmStringCreateSimple("Julia");
	radiobox = XmVaCreateSimpleRadioBox(rowcol, "radiobox", fractaltype, paramcallback, XmVaRADIOBUTTON, rbm, 0, NULL, NULL, XmVaRADIOBUTTON, rbj, 0, NULL, NULL, NULL);
*/
sep = XmCreateSeparatorGadget(rowcol, "sep", NULL, 0);
XtManageChild(sep);
n = 0;
XtSetArg(arg[n], XmNfractionBase, 5); n++;
form = XmCreateForm(rowcol, "form", arg, n);
ok = XmStringCreateLocalized("    Ok    "); n=0;
XtSetArg(arg[n], XmNlabelString, ok); n++;
XtSetArg(arg[n], XmNleftAttachment, XmATTACH_POSITION); n++;
XtSetArg(arg[n], XmNleftPosition, 1); n++;
XtSetArg(arg[n], XmNnavigationType, XmTAB_GROUP); n++;
okb = XmCreatePushButton(form, "obutton", arg, n);
XmStringFree(ok);
XtManageChild(okb);
cancel = XmStringCreateLocalized("  Cancel  "); n=0;
XtSetArg(arg[n], XmNlabelString, cancel); n++;
XtSetArg(arg[n], XmNleftAttachment, XmATTACH_POSITION); n++;
XtSetArg(arg[n], XmNleftPosition, 3); n++;
XtSetArg(arg[n], XmNnavigationType, XmTAB_GROUP); n++;
cancelb = XmCreatePushButton(form, "cbutton", arg, n);
XmStringFree(cancel);
XtManageChild(cancelb);
XtManageChild(form);
XtManageChild(rowcol);
XtManageChild(dialog);
XtAddCallback(cancelb, XmNactivateCallback, paramcallback, (XtPointer)100);
XtAddCallback(okb, XmNactivateCallback, paramcallback, (XtPointer)101);
XtRealizeWidget(dialog);
while (XtIsManaged(dialog)) { XtAppProcessEvent(app, XtIMAll); }
XSync(XtDisplay(dialog), 0);
}

void getfilename(char *name)
{
Widget dialog, toplevel;
XtAppContext app;
Arg arg[15];
XmString filter, title;
int sargc = 1;
char *sargv[] = {"fractald"};
int n = 0;

toplevel = XtVaOpenApplication(&app, "filename", NULL, 0, &sargc, sargv, NULL, sessionShellWidgetClass, NULL);
filter = XmStringCreateSimple("*.ppm");
title = XmStringCreateLocalized("Save To:");
//XtSetArg(arg[n], XmNdirectory, dir); n++; // to set directory
XtSetArg(arg[n], XmNdirMask, filter); n++;
XtSetArg(arg[n], XmNpathMode, XmPATH_MODE_RELATIVE); n++;
XtSetArg(arg[n], XmNdialogTitle, title); n++;
dialog = XmCreateFileSelectionDialog(toplevel, "getfilename", arg, n);
XtAddCallback(dialog, XmNcancelCallback, fileselcallback, 0);
XtAddCallback(dialog, XmNokCallback, fileselcallback, name);
XtManageChild(dialog);
XtUnmanageChild(XmFileSelectionBoxGetChild(dialog, XmDIALOG_HELP_BUTTON));
XmStringFree(filter);
XmStringFree(title);
while (XtIsManaged(dialog)) { XtAppProcessEvent(app, XtIMAll); }
XSync(XtDisplay(dialog), 0);
}

void messagebox(char *message)
{
Widget dialog, toplevel;
XtAppContext app;
Arg arg[15];
XmString xms, ok, title;
int sargc = 1;
char *sargv[] = {"fractald"};
int n = 0;

//XtSetLanguageProc(NULL, NULL, NULL);
toplevel = XtVaOpenApplication(&app, "messagebox", NULL, 0, &sargc, sargv, NULL, sessionShellWidgetClass, NULL);
xms = XmStringCreateLocalized(message);
title = XmStringCreateLocalized("Keyboard Controls");
ok = XmStringCreateLocalized("    Ok    ");
XtSetArg(arg[n], XmNmessageString, xms); n++;
XtSetArg(arg[n], XmNdialogTitle, title); n++;
XtSetArg(arg[n], XmNdialogType, XmDIALOG_INFORMATION); n++;
XtSetArg(arg[n], XmNdefaultPosition, False); n++;
XtSetArg(arg[n], XmNokLabelString, ok); n++;
#ifndef NO_XPM
XtSetArg(arg[n], XmNsymbolPixmap, icon); n++;
#endif
XtSetArg(arg[n], XmNx, 516); n++;
dialog = XmCreateInformationDialog(toplevel, "information", arg, n);
//dialog = XmCreateMessageDialog(toplevel, "information", arg, n);
XmStringFree(xms);
XmStringFree(title);
XmStringFree(ok);
XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_CANCEL_BUTTON));
XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON));
// can also do: XtUnmanageChild(XtNameToWidget(dialog, "Help"));
XtManageChild(dialog);
XtVaSetValues(dialog, XmNdialogStyle, XmDIALOG_APPLICATION_MODAL, NULL);
while (XtIsManaged(dialog)) { XtAppProcessEvent(app, XtIMAll); }
XSync(XtDisplay(dialog), 0);
//XmUpdateDisplay(toplevel);
}

	/* Linux / XWindows GUI stuff */

void ClearScreen()
{
memset(im, 0, 512*512*4);
memset(imaged, 0, 512*512*4);
//XPutImage(dpy, win, gc, ximage, 0, 0, 0, 0, ximage->width, ximage->height);
XClearWindow(dpy, win);
}

void SaveImage(char *name)
{
FILE *fp;
char str[256], str2[256];
unsigned char *p = im;

if (name==NULL) { getfilename(filename); return; }
if ((fp = fopen(name, "wb")) == NULL)
	{
	printf("Can't save to desired file\n");
	return;
	}
fprintf(fp, "P6\n512 512\n255\n");
for (n=0; n<512; n++)
	for (m=0; m<512; m++)
		{
		fputc(*(p+2), fp);
		fputc(*(p+1), fp);
		fputc(*(p), fp);
		p+=4; // 32-bit image buffer instead of 24
		}
fclose(fp);
sprintf(str2, "ls -al %s", name); system(str2);
}

int main(int argc, char *argv[])
{
int i, l;
char str[8], cmd[256];
XEvent event;
XKeyEvent kev;

graphinit();
if (argc == 1) { _Mandelbrot(); } // compute 1st location if no commandline args
// Wait for window to popup
//  Some X11 systems will crash if the window hasn't been mapped yet, so wait
//  for MapNotify event rather than Expose
do { XNextEvent(dpy, &event); } while (event.type != MapNotify);
//do { XNextEvent(dpy, &event); } while (event.type != Expose);
// fake keypresses so user can initialize the way they want
if (argc > 1) { strcpy(cmd, argv[1]); }
else { strcpy(cmd, "i"); } // just show computed image if no commandline args
kev.display = dpy;
kev.window = win;
kev.state = 0;
str[1] = '\0';
l = strlen(cmd);
for (i=0; i<l; i++)
	{
	kev.type = KeyPress;
	str[0] = cmd[i];
	// handle cases that don't work with Keysym/Keycode
	switch (str[0])
		{
		case '-': kev.keycode = 20; break;
		case '.': kev.keycode = 60; break;
		case ',': kev.keycode = 59; break;
		default: kev.keycode = XKeysymToKeycode(dpy, XStringToKeysym(str));
		}
	XSendEvent(dpy, win, True, KeyPressMask, (XEvent *)&kev);
	kev.type = KeyRelease;
	XSendEvent(dpy, win, True, KeyPressMask, (XEvent *)&kev);
	}
XFlush(dpy);
while (1)
	{
	/* trying to be nice on CPU - only continuously run while loop when
	   color cycling, otherwise, use XPeekEvent which waits for event */
	if ((cycle) && (!pauseit))
		{
		Cycle();
		if (speed > 1) { usleep(speed*500); }
		if (XPending(dpy)) { ProcessEvents(str); } 
		}
	else { if (XPeekEvent(dpy, &event)) { ProcessEvents(str); } }
	if (str[0] == 'q') { break; }
	}
}

int graphinit()
{
XGCValues gcvalues;
XSizeHints hints;
XWMHints wm_hints;
Visual *visual;
XVisualInfo vis;
XSetWindowAttributes wa;
int depth = 24;
int border_width = 0;
int screen_num;
unsigned int class;
unsigned long valuemask;
char *display_name = NULL;

/* I copied this routine from some other code I wrote.  I took out the
   unneeded (in this project) code I saw, but there still might be some more */
 
if ((dpy = XOpenDisplay(display_name)) == NULL)
	{
	printf("Cannot connect to X server %s\n", XDisplayName(display_name));
	exit(-1);
	}
screen_num = DefaultScreen(dpy);
depth = DefaultDepth(dpy, screen_num);
width = 512;
height = 513;  // some window decoration themes/window managers need extra
if (depth > 8)
	{
	//visual = DefaultVisual(dpy, screen_num);
	vis.depth = depth;
	XMatchVisualInfo(dpy, screen_num, vis.depth, TrueColor, &vis);
	visual = vis.visual;
	valuemask = CWColormap | CWBackPixel | CWBorderPixel;
	wa.background_pixel = BlackPixel(dpy, screen_num);
	wa.border_pixel = BlackPixel(dpy, screen_num);
	class = InputOutput;
	wa.colormap = XCreateColormap(dpy, RootWindow(dpy, screen_num), visual, AllocNone);
	win = XCreateWindow(dpy, RootWindow(dpy, screen_num),
		0, 0, width, height, border_width, vis.depth, class,
		visual, valuemask, &wa);
	wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, win, &wmDeleteMessage, 1);
	}
hints.flags = PPosition | PSize;
hints.x = hints.y = 0;
hints.width = width;
hints.height = height;
XSetNormalHints(dpy, win, &hints);
XStoreName(dpy, win, "Mark's x86 Asm. Fractals (Linux)");
XSetInputFocus(dpy, PointerRoot, RevertToNone, CurrentTime);
wm_hints.initial_state = NormalState;
wm_hints.input = True;
#ifndef NO_XPM
// IconPixmapHint method might only work in Gnome
XpmCreatePixmapFromData(dpy, win, myicon, &icon, NULL, NULL);
wm_hints.icon_pixmap = icon;
wm_hints.flags = StateHint | IconPixmapHint | InputHint;
#else
wm_hints.flags = StateHint | InputHint;
#endif
XSetWMHints(dpy, win, &wm_hints);
XSelectInput(dpy, win, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask | PointerMotionMask);
XMapWindow(dpy, win);
gc = XCreateGC(dpy, win, 0L, &gcvalues);
ximage = XCreateImage(dpy, visual, depth, ZPixmap, 0, 0, 512, 512, 8, 0);
if (ximage == NULL) { printf("Can't create image buffer\n"); return(1); }
//ximage->data = calloc(height, ximage->bytes_per_line);
ximage->data = im;
XSetForeground(dpy, gc, WhitePixel(dpy, 0));
}

int graphterm()
{
static int term = 0;

if (term) { return(1); }
XFlush(dpy);
if (ximage != NULL) { ximage->data = NULL; XDestroyImage(ximage); ximage = NULL; }
XFreeGC(dpy, gc);
XDestroyWindow(dpy, win);
XCloseDisplay(dpy);
term = 1;
}

int ProcessEvents(char *key)
{
XEvent event;
char str[512], string[128];
POINTS pt;
float ae, be, ae2, be2;
int dx, dy;

key[0] = string[0] = '\0';
if (XCheckTypedWindowEvent(dpy, win, Expose, &event))
	{
	XPutImage(dpy, win, gc, ximage, 0, 0, 0, 0, ximage->width, ximage->height);
	}
if (XCheckTypedWindowEvent(dpy, win, ClientMessage, &event))
	{
	if (event.xclient.data.l[0] == wmDeleteMessage)
		{
		key[0] = 'q'; graphterm(); return(1);
		}
	}
if (XCheckTypedWindowEvent(dpy, win, KeyPress, &event))
	{
	XLookupString(&event.xkey, string, 128, NULL, NULL);
	switch (event.xkey.keycode)
		{
		case 113: speed-=dspeed; if (speed < 0) { speed = 0; } break; // left
		case 114: speed+=dspeed; break; // right
		}
	key[0] = string[0];
	switch (string[0])
		{
		case '0': Choice0(); break;
		case '1': Choice1(); break;
		case '2': Choice2(); break;
		case '3': Choice3(); break;
		case '4': Choice4(); break;
		case '5': Choice5(); break;
		case '6': Choice6(); break;
		case '7': Choice7(); break;
		case '8': Choice8(); break;
		case '9': Choice9(); break;
		case 'a': Choicea(); break;
		case 'b': Choiceb(); ClearScreen(); break;
		case '-': Choicem(); break;
		case 'q': graphterm(); return(1); break;
		case 'c': cycle = !cycle; break;
		case 'd': if (cydir == 1) { cydir = -1; } else { cydir = 1; } break;
		case ' ': pauseit = !pauseit; break;
		case ',': cyinc-=2; break;
		case '.': cyinc+=2; break;
		case 'z': zoomgetpoint = 1; zoomtype = 0; pt0.x = -1;
			XSetFunction(dpy, gc, GXxor); break;
		case 'x': zoomgetpoint = 1; zoomtype = 1; pt0.x = -1;
			XSetFunction(dpy, gc, GXxor); break;
		case 'u': aedge = aedgeUndo; bedge = bedgeUndo;
			xdelta = xdeltaUndo; ydelta = ydeltaUndo;
			if (fractaltype == MANDELBROT) { Mandelbrot(); } else { Julia(); }
			break;
		case 'k': keepitsquare = !keepitsquare; break;
		case 'p': parameters(); break;
		case 'r': if (fractaltype == MANDELBROT) { Mandelbrot(); } else { Julia(); } break;
		case 'i': XPutImage(dpy, win, gc, ximage, 0, 0, 0, 0, ximage->width, ximage->height); break;
		case 's': SaveImage(NULL); break;
		case 'h':
#ifdef HAVE_SDL2
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
				"Keyboard Controls",
#else
			messagebox(
#endif
"\
1 - Mandelbrot location 1\n\
2 - Mandelbrot location 2\n\
3 - Mandelbrot location 3\n\
4 - Julia location 1\n\
5 - Julia location 2\n\
6 - Julia location 3\n\
7 - Julia location 4\n\
8 - Julia location 5\n\
9 - Julia location 6\n\
0 - Julia location 7\n\
a - Julia location 8\n\
b - Toggle size\n\
- - Mandelbrot location 4\n\
r - Recalculate/redraw (use after 'b', for example)\n\
z - Zoom by opposing corners\n\
x - Zoom by center to corner\n\
u - Undo zoom\n\
k - Toggle zoom square/free\n\
c - Toggle color cycle\n\
d - Change cycle direction\n\
, - Decrease cycle delta\n\
. - Increase cycle delta\n\
s - Save image\n\
{left} - Increase cycle speed\n\
{right} - Decrease cycle speed\n\
{space} - Pause cycle\n\
h - Help\n\
q - Quit\n\
"
#ifdef HAVE_SDL2
,NULL); // printf would be ok with this, but wanted to avoid compiler warning
#else
);
#endif
			break;
		}
	}
if (XCheckTypedWindowEvent(dpy, win, ConfigureNotify, &event))
	{
	if ((event.xconfigure.width != width)||(event.xconfigure.height != height))
		{
		width = event.xconfigure.width;
		height = event.xconfigure.height;
		XPutImage(dpy, win, gc, ximage, 0, 0, 0, 0, ximage->width, ximage->height);
		}
	}
if (XCheckTypedWindowEvent(dpy, win, ButtonPress, &event))
	{
	XButtonEvent *e = (XButtonEvent *)&event;
	if (e->button == 1)
		{
		if (zoomgetpoint == 1)
			{
			if (pt0.x >= 0)
				{
				XDrawLine(dpy, win, gc, 0, pt0.y, 512, pt0.y);
				XDrawLine(dpy, win, gc, pt0.x, 0, pt0.x, 512);
				}
			pts.x = e->x; pts.y = e->y; zoomgetpoint = 2; pte.x = -1;
			}
		}
	}
if (XCheckTypedWindowEvent(dpy, win, ButtonRelease, &event))
	{
	XButtonEvent *e = (XButtonEvent *)&event;
	if (e->button == 1)
		{
		if (zoomgetpoint == 2)
			{
			XSetFunction(dpy, gc, GXcopy);
			aedgeUndo = aedge; bedgeUndo = bedge;
			xdeltaUndo = xdelta; ydeltaUndo = ydelta;
			if (zoomtype)
				{
				dx = abs(pte.x-pts.x); dy = abs(pte.y-pts.y);
				ae = aedge+xdelta*(float)(pts.x-dx)/(float)(512);
				be = bedge+ydelta*(float)(pts.y-dy)/(float)(512);
				ae2 = aedge+xdelta*(float)(pts.x+dx)/(float)(512);
				be2 = bedge+ydelta*(float)(pts.y+dy)/(float)(512);
				}
			else
				{
				ae = aedge+xdelta*(float)(pts.x)/(float)(512);
				be = bedge+ydelta*(float)(pts.y)/(float)(512);
				ae2 = aedge+xdelta*(float)(pte.x)/(float)(512);
				be2 = bedge+ydelta*(float)(pte.y)/(float)(512);
				}
			aedge = ae; bedge = be;
			xdelta = ae2-ae; ydelta = be2-be;
			zoomgetpoint = 0;
			if (fractaltype == MANDELBROT) { Mandelbrot(); } else { Julia(); }
			XStoreName(dpy, win, "Mark's x86 Asm. Fractals (Linux)");
			}
		}
	}
if (XCheckTypedWindowEvent(dpy, win, MotionNotify, &event))
	{
	XMotionEvent *e = (XMotionEvent *)&event;
	if (zoomgetpoint == 1)
		{
		pt.x = e->x; pt.y = e->y;
		if (pt0.x >= 0)
			{
			XDrawLine(dpy, win, gc, 0, pt0.y, 512, pt0.y);
			XDrawLine(dpy, win, gc, pt0.x, 0, pt0.x, 512);
			}
		XDrawLine(dpy, win, gc, 0, pt.y, 512, pt.y);
		XDrawLine(dpy, win, gc, pt.x, 0, pt.x, 512);
		ae = aedge+xdelta*(float)(pt.x)/(float)(512);
		be = bedge+ydelta*(float)(pt.y)/(float)(512);
		sprintf(str, "%.10f %.10f", ae, be);
		XStoreName(dpy, win, str);
		pt0 = pt;
		}
	else if (zoomgetpoint == 2)
		{
		if (zoomtype)
			{
			if (pte.x >= 0)
				{
				dx = abs(pte.x-pts.x); dy = abs(pte.y-pts.y);
				XDrawLine(dpy, win, gc, pts.x-dx, pts.y-dy, pts.x+dy, pts.y-dy);
				XDrawLine(dpy, win, gc, pts.x+dx, pts.y-dy+1, pts.x+dx, pts.y+dy-1);
				XDrawLine(dpy, win, gc, pts.x-dx, pts.y-dy+1, pts.x-dx, pts.y+dy-1);
				XDrawLine(dpy, win, gc, pts.x-dx, pts.y+dy, pts.x+dx, pts.y+dy);
				}
			pte.x = e->x; pte.y = e->y;
			if (keepitsquare) { pte.y = pts.y+(pte.x-pts.x); }
			dx = abs(pte.x-pts.x); dy = abs(pte.y-pts.y);
			XDrawLine(dpy, win, gc, pts.x-dx, pts.y-dy, pts.x+dy, pts.y-dy);
			XDrawLine(dpy, win, gc, pts.x+dx, pts.y-dy+1, pts.x+dx, pts.y+dy-1);
			XDrawLine(dpy, win, gc, pts.x-dx, pts.y-dy+1, pts.x-dx, pts.y+dy-1);
			XDrawLine(dpy, win, gc, pts.x-dx, pts.y+dy, pts.x+dx, pts.y+dy);
			ae = aedge+xdelta*(float)(pts.x-dx)/(float)(512);
			be = bedge+ydelta*(float)(pts.y-dy)/(float)(512);
			ae2 = aedge+xdelta*(float)(pts.x+dx)/(float)(512);
			be2 = bedge+ydelta*(float)(pts.y+dy)/(float)(512);
			}
		else
			{
			if (pte.x >= 0)
				{
				XDrawLine(dpy, win, gc, pts.x, pts.y, pte.x, pts.y);
				XDrawLine(dpy, win, gc, pts.x, pte.y-1, pts.x, pts.y+1);
				XDrawLine(dpy, win, gc, pte.x, pts.y+1, pte.x, pte.y-1);
				XDrawLine(dpy, win, gc, pte.x, pte.y, pts.x, pte.y);
				}
			pte.x = e->x; pte.y = e->y;
			if (keepitsquare) { pte.y = pts.y+(pte.x-pts.x); }
			XDrawLine(dpy, win, gc, pts.x, pts.y, pte.x, pts.y);
			XDrawLine(dpy, win, gc, pts.x, pte.y-1, pts.x, pts.y+1);
			XDrawLine(dpy, win, gc, pte.x, pts.y+1, pte.x, pte.y-1);
			XDrawLine(dpy, win, gc, pte.x, pte.y, pts.x, pte.y);
			ae = aedge+xdelta*(float)(pts.x)/(float)(512);
			be = bedge+ydelta*(float)(pts.y)/(float)(512);
			ae2 = aedge+xdelta*(float)(pte.x)/(float)(512);
			be2 = bedge+ydelta*(float)(pte.y)/(float)(512);
			}
		sprintf(str, "%.10f %.10f to %.10f %.10f (%.10f %.10f)",
			ae, be, ae2, be2, ae2-ae, be2-be);
		XStoreName(dpy, win, str);
		}
	}
}
