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
			fld qword [xdelta]
			fdiv qword [xres]
			fstp qword [xgap]		; xgap=xdelta/xres
			fld qword [ydelta]
			fdiv qword [yres]
			fstp qword [ygap]		; ygap=ydelta/yres
			fldz
Loopm:		fst qword [m]			; for (m=0;m<yres;m++)
			fmul qword [ygap]
			fadd qword [bedge]
			fstp qword [bc]			; bc=m*ygap+bedge
			fldz
Loopn:		fst qword [n]			; for (n=0;n<xres;n++)
			fmul qword [xgap]
			fadd qword [aedge]
			fstp qword [ac]			; ac=n*xgap+aedge
			fldz
			fst qword [az]			; az=0
			fst qword [bz]			; bz=0
			fstp qword [sizev]		; sizev=0 (size must be reserved word)
			mov dword [count],0		; count=0
Loopw:		fld qword [sizev]
			fcomp qword [converge]
			fnstsw ax
			test ah,65
			je Exitw
			mov eax,[limit]
			cmp [count],eax			; was cmp count,limit when limit was #define
			jg Exitw				; while (size<=converge && count<=limit)
			fld qword [az]
			fst qword [temp]		; temp=az
			fmul qword [az]
			fadd qword [ac]
			fstp qword [az]
			fld qword [bz]
			fmul qword [bz]
			fstp qword [tempb]
			fld qword [az]
			fsub qword [tempb]
			fstp qword [az]			; az=az*az-bz*bz+ac
			fld qword [bz]
			fmul qword [temp]
			fmul qword [two]
			fadd qword [bc]
			fstp qword [bz]			; bz=temp*bz*2+bc
			add dword [count],1		; count++
			fld qword [az]
			fmul qword [az]
			fstp qword [tempb]
			fld qword [bz]
			fmul qword [bz]
			fadd qword [tempb]
			fstp qword [sizev]		; size=(az*az+bz*bz)
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
			fld qword [n]
			fadd qword [one]
			fst qword [n]
			fcom qword [xres]
			fnstsw ax
			test ah,1
			jne Loopn				; end Loop n
			mov eax,[resadd]
			add [pixel],eax
			fstp qword [popped]
			fld qword [m]
			fadd qword [one]
			fst qword [m]
			fcom qword [yres]
			fnstsw ax
			test ah,1
			jne Loopm				; end Loop m
			fstp qword [popped]
			;call Cycle
			ret
			
global _Julia
_Julia:		mov dword [pixel],0
			fld qword [xdelta]
			fdiv qword [xres]
			fstp qword [xgap]		; xgap=xdelta/xres
			fld qword [ydelta]
			fdiv qword [yres]
			fstp qword [ygap]		; ygap=ydelta/yres
			fldz
Loopmj:		fst qword [m]			; for (m=0;m<yres;m++)
			fmul qword [ygap]
			fadd qword [bedge]
			fstp qword [bc]			; bc=m*ygap+bedge
			fldz
Loopnj:		fst qword [n]			; for (n=0;n<xres;n++)
			fmul qword [xgap]
			fadd qword [aedge]
			fst qword [ac]			; ac=n*xgap+aedge
			fstp qword [az]			; az=ac
			fld qword [bc]
			fstp qword [bz]			; bz=bc
			fldz
			fstp qword [sizev]		; sizev=0 (size must be reserved word)
			mov dword [count],0		; count=0
Loopwj:		fld qword [sizev]
			fcomp qword [converge]
			fnstsw ax
			test ah,65
			je Exitwj
			mov eax,[limit]
			cmp [count],eax
			jg Exitwj				; while (size<=converge && count<=limit)
			fld qword [az]
			fst qword [temp]		; temp=az
			fmul qword [az]
			fadd qword [a]
			fstp qword [az]
			fld qword [bz]
			fmul qword [bz]
			fstp qword [tempb]
			fld qword [az]
			fsub qword [tempb]
			fstp qword [az]			; az=az*az-bz*bz+a
			fld qword [bz]
			fmul qword [temp]
			fmul qword [two]
			fadd qword [b]
			fstp qword [bz]			; bz=temp*bz*2+b
			add dword [count],1		; count++
			fld qword [az]
			fmul qword [az]
			fstp qword [tempb]
			fld qword [bz]
			fmul qword [bz]
			fadd qword [tempb]
			fstp qword [sizev]		; size=(az*az+bz*bz)
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
			fld qword [n]
			fadd qword [one]
			fst qword [n]
			fcom qword [xres]
			fnstsw ax
			test ah,1
			jne Loopnj				; end Loop n
			mov eax,[resadd]
			add [pixel],eax
			fstp qword [popped]
			fld qword [m]
			fadd qword [one]
			fst qword [m]
			fcom qword [yres]
			fnstsw ax
			test ah,1
			jne Loopmj				; end Loop m
			fstp qword [popped]
			;call Cycle
			ret

global Choice1
Choice1:	fld qword [aedge1]
			fstp qword [aedge]
			fld qword [bedge1]
			fstp qword [bedge]
			fld qword [xdelta1]
			fstp qword [xdelta]
			fld qword [ydelta1]
			fstp qword [ydelta]
			mov dword [fractaltype],MANDELBROT
			call Mandelbrot
			ret

global Choice2
Choice2:	fld qword [aedge2]
			fstp qword [aedge]
			fld qword [bedge2]
			fstp qword [bedge]
			fld qword [xdelta2]
			fstp qword [xdelta]
			fld qword [ydelta2]
			fstp qword [ydelta]
			mov dword [fractaltype],MANDELBROT
			call Mandelbrot
			ret

global Choice3
Choice3:	fld qword [aedge3]
			fstp qword [aedge]
			fld qword [bedge3]
			fstp qword [bedge]
			fld qword [xdelta3]
			fstp qword [xdelta]
			fld qword [ydelta3]
			fstp qword [ydelta]
			mov dword [fractaltype],MANDELBROT
			call Mandelbrot
			ret

global Choice4
Choice4:	fld qword [aedge4]
			fstp qword [aedge]
			fld qword [bedge4]
			fstp qword [bedge]
			fld qword [xdelta4]
			fstp qword [xdelta]
			fld qword [ydelta4]
			fstp qword [ydelta]
			fld qword [a1]
			fstp qword [a]
			fld qword [b1]
			fstp qword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice5
Choice5:	fld qword [aedge4]
			fstp qword [aedge]
			fld qword [bedge4]
			fstp qword [bedge]
			fld qword [xdelta4]
			fstp qword [xdelta]
			fld qword [ydelta4]
			fstp qword [ydelta]
			fld qword [a2]
			fstp qword [a]
			fld qword [b2]
			fstp qword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice6
Choice6:	fld qword [aedge4]
			fstp qword [aedge]
			fld qword [bedge4]
			fstp qword [bedge]
			fld qword [xdelta4]
			fstp qword [xdelta]
			fld qword [ydelta4]
			fstp qword [ydelta]
			fld qword [a3]
			fstp qword [a]
			fld qword [b3]
			fstp qword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice7
Choice7:	fld qword [aedge4]
			fstp qword [aedge]
			fld qword [bedge4]
			fstp qword [bedge]
			fld qword [xdelta4]
			fstp qword [xdelta]
			fld qword [ydelta4]
			fstp qword [ydelta]
			fld qword [a4]
			fstp qword [a]
			fld qword [b4]
			fstp qword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice8
Choice8:	fld qword [aedge4]
			fstp qword [aedge]
			fld qword [bedge4]
			fstp qword [bedge]
			fld qword [xdelta4]
			fstp qword [xdelta]
			fld qword [ydelta4]
			fstp qword [ydelta]
			fld qword [a5]
			fstp qword [a]
			fld qword [b5]
			fstp qword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice9
Choice9:	fld qword [aedge4]
			fstp qword [aedge]
			fld qword [bedge4]
			fstp qword [bedge]
			fld qword [xdelta4]
			fstp qword [xdelta]
			fld qword [ydelta4]
			fstp qword [ydelta]
			fld qword [a6]
			fstp qword [a]
			fld qword [b6]
			fstp qword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choice0
Choice0:	fld qword [aedge4]
			fstp qword [aedge]
			fld qword [bedge4]
			fstp qword [bedge]
			fld qword [xdelta4]
			fstp qword [xdelta]
			fld qword [ydelta4]
			fstp qword [ydelta]
			fld qword [a7]
			fstp qword [a]
			fld qword [b7]
			fstp qword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choicea
Choicea:	fld qword [aedge4]
			fstp qword [aedge]
			fld qword [bedge4]
			fstp qword [bedge]
			fld qword [xdelta4]
			fstp qword [xdelta]
			fld qword [ydelta4]
			fstp qword [ydelta]
			fld qword [a8]
			fstp qword [a]
			fld qword [b8]
			fstp qword [b]
			mov dword [fractaltype],JULIA
			call Julia
			ret

global Choiceb
Choiceb:	fld qword [xres]
			fcomp qword [xres1]
			fnstsw ax
			test ah,64
			jne To128
			fld qword [xres1]
			fstp qword [xres]
			fld qword [yres1]
			fstp qword [yres]
			mov eax,[numpixels1]
			mov [numpixels],eax
			mov eax,[resadd1]
			mov [resadd],eax
			jmp resend
To128:		fld qword [xres2]
			fstp qword [xres]
			fld qword [yres2]
			fstp qword [yres]
			mov eax,[numpixels2]
			mov [numpixels],eax
			mov eax,[resadd2]
			mov [resadd],eax
resend:
			ret

global Choicem
Choicem:	fld qword [aedge5]
			fstp qword [aedge]
			fld qword [bedge5]
			fstp qword [bedge]
			fld qword [xdelta5]
			fstp qword [xdelta]
			fld qword [ydelta5]
			fstp qword [ydelta]
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
