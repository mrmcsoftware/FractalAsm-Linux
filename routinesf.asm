extern xdelta,ydelta,pixel,xres,yres,xgap,ygap,aedge,bedge,ac,bc,az,bz,sizev
extern count,converge,limit,temp,tempb,one,two,ncols,im,imaged,Colors,popped
extern resadd,resadd1,resadd2,m,n,fractaltype,a,b,xres1,yres1,xres2,yres2
extern aedge1,bedge1,xdelta1,ydelta1
extern aedge2,bedge2,xdelta2,ydelta2
extern aedge3,bedge3,xdelta3,ydelta3
extern aedge4,bedge4,xdelta4,ydelta4
extern aedge5,bedge5,xdelta5,ydelta5
extern a1,b1,a2,b2,a3,b3,a4,b4,a5,b5,a6,b6,a7,b7,a8,b8
extern numpixels,numpixels1,numpixels2
extern cydir,cyinc,off

extern Mandelbrot
extern Julia

%define MANDELBROT 0
%define JULIA 1

global _Mandelbrot
_Mandelbrot:
			mov dword [pixel],0
			fld dword [xdelta]
			fdiv dword [xres]
			fstp dword [xgap]		; xgap=xdelta/xres
			fld dword [ydelta]
			fdiv dword [yres]
			fstp dword [ygap]		; ygap=ydelta/yres
			fldz
Loopm:		fst dword [m]			; for (m=0;m<yres;m++)
			fmul dword [ygap]
			fadd dword [bedge]
			fstp dword [bc]			; bc=m*ygap+bedge
			fldz
Loopn:		fst dword [n]			; for (n=0;n<xres;n++)
			fmul dword [xgap]
			fadd dword [aedge]
			fstp dword [ac]			; ac=n*xgap+aedge
			fldz
			fst dword [az]			; az=0
			fst dword [bz]			; bz=0
			fstp dword [sizev]		; sizev=0 (size must be reserved word)
			mov dword [count],0		; count=0
Loopw:		fld dword [sizev]
			fcomp dword [converge]
			fnstsw ax
			test ah,65
			je Exitw
			mov eax,[limit]
			cmp [count],eax			; was cmp count,limit when limit was #define
			jg Exitw				; while (size<=converge && count<=limit)
			fld dword [az]
			fst dword [temp]		; temp=az
			fmul dword [az]
			fadd dword [ac]
			fstp dword [az]
			fld dword [bz]
			fmul dword [bz]
			fstp dword [tempb]
			fld dword [az]
			fsub dword [tempb]
			fstp dword [az]			; az=az*az-bz*bz+ac
			fld dword [bz]
			fmul dword [temp]
			fmul dword [two]
			fadd dword [bc]
			fstp dword [bz]			; bz=temp*bz*2+bc
			add dword [count],1		; count++
			fld dword [az]
			fmul dword [az]
			fstp dword [tempb]
			fld dword [bz]
			fmul dword [bz]
			fadd dword [tempb]
			fstp dword [sizev]		; size=(az*az+bz*bz)
			jmp Loopw				; end while
Exitw:		mov eax,[limit]
			cmp [count],eax
			jle ok
			mov dword [count],0		; if (count>limit) count=0
			jmp ok2
ok:			mov eax,[off]
			add [count],eax
			mov eax,[count]			;
			mov edx,0				;
			idiv dword [ncols]		; MODI 255
			mov [count],edx			;
			add dword [count],1		; else count=(count+off)%255+1
ok2:		mov eax,[count]
			mov ebx,[pixel]
			mov [imaged+ebx],eax	; save count for color cycling (not aligned)
			imul eax,3
			add eax,2
			mov cl,[Colors+eax]
			sub eax,1
			mov ebx,[pixel]
			mov [im+ebx],cl
			add dword [pixel],1
			mov cl,[Colors+eax]
			sub eax,1
			mov ebx,[pixel]
			mov [im+ebx],cl
			add dword [pixel],1
			mov cl,[Colors+eax]
			mov ebx,[pixel]
			mov [im+ebx],cl
			add dword [pixel],1
			add dword [pixel],1		; 32-bit image buffer instead of 24
			fld dword [n]
			fadd dword [one]
			fst dword [n]
			fcom dword [xres]
			fnstsw ax
			test ah,1
			jne Loopn				; end Loop n
			mov eax,[resadd]
			add [pixel],eax
			fstp dword [popped]
			fld dword [m]
			fadd dword [one]
			fst dword [m]
			fcom dword [yres]
			fnstsw ax
			test ah,1
			jne Loopm				; end Loop m
			fstp dword [popped]
			;call Cycle
			ret
			
global _Julia
_Julia:		mov dword [pixel],0
			fld dword [xdelta]
			fdiv dword [xres]
			fstp dword [xgap]		; xgap=xdelta/xres
			fld dword [ydelta]
			fdiv dword [yres]
			fstp dword [ygap]		; ygap=ydelta/yres
			fldz
Loopmj:		fst dword [m]			; for (m=0;m<yres;m++)
			fmul dword [ygap]
			fadd dword [bedge]
			fstp dword [bc]			; bc=m*ygap+bedge
			fldz
Loopnj:		fst dword [n]			; for (n=0;n<xres;n++)
			fmul dword [xgap]
			fadd dword [aedge]
			fst dword [ac]			; ac=n*xgap+aedge
			fstp dword [az]			; az=ac
			fld dword [bc]
			fstp dword [bz]			; bz=bc
			fldz
			fstp dword [sizev]		; sizev=0 (size must be reserved word)
			mov dword [count],0		; count=0
Loopwj:		fld dword [sizev]
			fcomp dword [converge]
			fnstsw ax
			test ah,65
			je Exitwj
			mov eax,[limit]
			cmp [count],eax
			jg Exitwj				; while (size<=converge && count<=limit)
			fld dword [az]
			fst dword [temp]		; temp=az
			fmul dword [az]
			fadd dword [a]
			fstp dword [az]
			fld dword [bz]
			fmul dword [bz]
			fstp dword [tempb]
			fld dword [az]
			fsub dword [tempb]
			fstp dword [az]			; az=az*az-bz*bz+a
			fld dword [bz]
			fmul dword [temp]
			fmul dword [two]
			fadd dword [b]
			fstp dword [bz]			; bz=temp*bz*2+b
			add dword [count],1		; count++
			fld dword [az]
			fmul dword [az]
			fstp dword [tempb]
			fld dword [bz]
			fmul dword [bz]
			fadd dword [tempb]
			fstp dword [sizev]		; size=(az*az+bz*bz)
			jmp Loopwj				; end while
Exitwj:		mov eax,[limit]
			cmp [count],eax
			jle okj
			mov dword [count],0		; if (count>limit) count=0
			jmp ok2j
okj:		mov eax,[off]
			add dword [count],eax
			mov eax,[count]			;
			mov edx,0				;
			idiv dword [ncols]		; MODI 255
			mov [count],edx			;
			add dword [count],1		; else count=(count+off)%255+1
ok2j:		mov eax,[count]
			mov ebx,[pixel]
			mov [imaged+ebx],eax	; save count for color cycling (not aligned)
			imul eax,3
			add eax,2
			mov cl,[Colors+eax]
			sub eax,1
			mov ebx,[pixel]
			mov [im+ebx],cl
			add dword [pixel],1
			mov cl,[Colors+eax]
			sub eax,1
			mov ebx,[pixel]
			mov [im+ebx],cl
			add dword [pixel],1
			mov cl,[Colors+eax]
			mov ebx,[pixel]
			mov [im+ebx],cl
			add dword [pixel],1
			add dword [pixel],1		; 32-bit image buffer instead of 24
			fld dword [n]
			fadd dword [one]
			fst dword [n]
			fcom dword [xres]
			fnstsw ax
			test ah,1
			jne Loopnj				; end Loop n
			mov eax,[resadd]
			add [pixel],eax
			fstp dword [popped]
			fld dword [m]
			fadd dword [one]
			fst dword [m]
			fcom dword [yres]
			fnstsw ax
			test ah,1
			jne Loopmj				; end Loop m
			fstp dword [popped]
			;call Cycle
			ret

global Choice1
Choice1:	fld dword [aedge1]
			fstp dword [aedge]
			fld dword [bedge1]
			fstp dword [bedge]
			fld dword [xdelta1]
			fstp dword [xdelta]
			fld dword [ydelta1]
			fstp dword [ydelta]
			mov dword [fractaltype],MANDELBROT
			call Mandelbrot
			ret

global Choice2
Choice2:	fld dword [aedge2]
			fstp dword [aedge]
			fld dword [bedge2]
			fstp dword [bedge]
			fld dword [xdelta2]
			fstp dword [xdelta]
			fld dword [ydelta2]
			fstp dword [ydelta]
			mov dword [fractaltype],MANDELBROT
			call Mandelbrot
			ret

global Choice3
Choice3:	fld dword [aedge3]
			fstp dword [aedge]
			fld dword [bedge3]
			fstp dword [bedge]
			fld dword [xdelta3]
			fstp dword [xdelta]
			fld dword [ydelta3]
			fstp dword [ydelta]
			mov dword [fractaltype],MANDELBROT
			call Mandelbrot
			ret

global Choice4
Choice4:	fld dword [aedge4]
			fstp dword [aedge]
			fld dword [bedge4]
			fstp dword [bedge]
			fld dword [xdelta4]
			fstp dword [xdelta]
			fld dword [ydelta4]
			fstp dword [ydelta]
			fld dword [a1]
			fstp dword [a]
			fld dword [b1]
			fstp dword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice5
Choice5:	fld dword [aedge4]
			fstp dword [aedge]
			fld dword [bedge4]
			fstp dword [bedge]
			fld dword [xdelta4]
			fstp dword [xdelta]
			fld dword [ydelta4]
			fstp dword [ydelta]
			fld dword [a2]
			fstp dword [a]
			fld dword [b2]
			fstp dword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice6
Choice6:	fld dword [aedge4]
			fstp dword [aedge]
			fld dword [bedge4]
			fstp dword [bedge]
			fld dword [xdelta4]
			fstp dword [xdelta]
			fld dword [ydelta4]
			fstp dword [ydelta]
			fld dword [a3]
			fstp dword [a]
			fld dword [b3]
			fstp dword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice7
Choice7:	fld dword [aedge4]
			fstp dword [aedge]
			fld dword [bedge4]
			fstp dword [bedge]
			fld dword [xdelta4]
			fstp dword [xdelta]
			fld dword [ydelta4]
			fstp dword [ydelta]
			fld dword [a4]
			fstp dword [a]
			fld dword [b4]
			fstp dword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice8
Choice8:	fld dword [aedge4]
			fstp dword [aedge]
			fld dword [bedge4]
			fstp dword [bedge]
			fld dword [xdelta4]
			fstp dword [xdelta]
			fld dword [ydelta4]
			fstp dword [ydelta]
			fld dword [a5]
			fstp dword [a]
			fld dword [b5]
			fstp dword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice9
Choice9:	fld dword [aedge4]
			fstp dword [aedge]
			fld dword [bedge4]
			fstp dword [bedge]
			fld dword [xdelta4]
			fstp dword [xdelta]
			fld dword [ydelta4]
			fstp dword [ydelta]
			fld dword [a6]
			fstp dword [a]
			fld dword [b6]
			fstp dword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice0
Choice0:	fld dword [aedge4]
			fstp dword [aedge]
			fld dword [bedge4]
			fstp dword [bedge]
			fld dword [xdelta4]
			fstp dword [xdelta]
			fld dword [ydelta4]
			fstp dword [ydelta]
			fld dword [a7]
			fstp dword [a]
			fld dword [b7]
			fstp dword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choicea
Choicea:	fld dword [aedge4]
			fstp dword [aedge]
			fld dword [bedge4]
			fstp dword [bedge]
			fld dword [xdelta4]
			fstp dword [xdelta]
			fld dword [ydelta4]
			fstp dword [ydelta]
			fld dword [a8]
			fstp dword [a]
			fld dword [b8]
			fstp dword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choiceb
Choiceb:	fld dword [xres]
			fcomp dword [xres1]
			fnstsw ax
			test ah,64
			jne To128
			fld dword [xres1]
			fstp dword [xres]
			fld dword [yres1]
			fstp dword [yres]
			mov eax,[numpixels1]
			mov [numpixels],eax
			mov eax,[resadd1]
			mov [resadd],eax
			jmp resend
To128:		fld dword [xres2]
			fstp dword [xres]
			fld dword [yres2]
			fstp dword [yres]
			mov eax,[numpixels2]
			mov [numpixels],eax
			mov eax,[resadd2]
			mov [resadd],eax
resend:
			ret

global Choicem
Choicem:	fld dword [aedge5]
			fstp dword [aedge]
			fld dword [bedge5]
			fstp dword [bedge]
			fld dword [xdelta5]
			fstp dword [xdelta]
			fld dword [ydelta5]
			fstp dword [ydelta]
			mov dword [fractaltype],MANDELBROT
			call Mandelbrot
			ret

global _Cycle
_Cycle:
Loopc0:		mov eax,[cyinc]
			mul dword [cydir]
			add [off],eax
			mov dword [pixel],0
Loopc:		mov ebx,[pixel]
			mov ecx,[imaged+ebx]
			mov [count],ecx
			cmp dword [count],0
			je ok3
			mov eax,[off]
			add [count],eax
			mov eax,[count]
			mov edx,0
			idiv dword [ncols]
			mov [count],edx
			add dword [count],1
ok3:		mov eax,[count]
			imul eax,3
			add eax,2
			mov cl,[Colors+eax]
			sub eax,1
			mov ebx,[pixel]
			mov [im+ebx],cl
			add dword [pixel],1
			mov cl,[Colors+eax]
			sub eax,1
			mov ebx,[pixel]
			mov [im+ebx],cl
			add dword [pixel],1
			mov cl,[Colors+eax]
			mov ebx,[pixel]
			mov [im+ebx],cl
			add dword [pixel],1
			add dword [pixel],1		; 32-bit image buffer instead of 24
			mov eax,[numpixels]
			cmp [pixel],eax
			jl Loopc
			ret
