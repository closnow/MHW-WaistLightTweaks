option casemap:none
EXTERN g_ScoutFlyReturn:QWORD
EXTERN g_WaistReturn:QWORD
EXTERN light_flag:DWORD
EXTERN cfg_guide_brightness:REAL4
EXTERN cfg_waist_cage_brightness:REAL4
EXTERN cfg_waist_cage_range:REAL4
EXTERN cfg_waist_light_brightness:REAL4
EXTERN cfg_waist_light_range:REAL4
EXTERN cfg_base_cage_brightness:REAL4
EXTERN cfg_base_cage_range:REAL4
EXTERN cfg_base_waist_brightness:REAL4
EXTERN cfg_base_waist_range:REAL4

.code
public ScoutFlyHook
public WaistHook

;-----------------------------------------
; 導蟲ケージ＋ガイド導蟲
;-----------------------------------------
ScoutFlyHook PROC
    cmp dword ptr [rcx+140h], 0FFCDFFE4h
    je guide_scoutfly
    cmp dword ptr [rcx+140h], 0FF030403h
    je cage_field
    cmp dword ptr [rcx+140h], 0FF202B26h
    jne exit_cage
    cmp dword ptr [rcx-1Ch], 0FFFFFC40h
    jne exit_cage
    jmp cage_base

guide_scoutfly:
    push rax
    mov eax, dword ptr [cfg_guide_brightness]
    mov dword ptr [rcx+144h], eax             ; ガイド導蟲の明るさ
    pop rax
    xorps xmm3, xmm3
    jmp exit_cage

cage_field:
    push rax
    mov eax, dword ptr [cfg_waist_cage_range]
    mov dword ptr [rcx+588h], eax             ; 導蟲ケージの照射範囲（フィールド）
    pop rax
    
    test dword ptr [light_flag], 1
    jne cage_off

    push rax
    mov eax, dword ptr [cfg_waist_cage_brightness]
    mov dword ptr [rcx+144h], eax             ; 導蟲ケージの明るさ（フィールド）
    pop rax
    
    movss xmm3, dword ptr [rcx+144h]
    jmp exit_cage

cage_base:
    push rax
    mov eax, dword ptr [cfg_base_cage_range]
    mov dword ptr [rcx+588h], eax             ; 導蟲ケージの照射範囲（拠点）
    pop rax
   
    test dword ptr [light_flag], 1
    jne cage_off

    push rax
    mov eax, dword ptr [cfg_base_cage_brightness]
    mov dword ptr [rcx+144h], eax             ; 導蟲ケージの明るさ（拠点）
    pop rax
    
    movss xmm3, dword ptr [rcx+144h]
    jmp exit_cage

cage_off:
    mov dword ptr [rcx+144h], 0
    xorps xmm3, xmm3

exit_cage:
    movss xmm3, dword ptr [rcx+144h]
    jmp qword ptr [g_ScoutFlyReturn]
ScoutFlyHook ENDP

;-----------------------------------------
; 腰ライト（フィールド／拠点）
;-----------------------------------------
WaistHook PROC
    mov ecx, [rcx+140h]
    movss xmm3, dword ptr [rbx+144h]

    cmp ecx, 0FF8EA399h
    je waist_field
    cmp ecx, 0FF7AB799h
    jne exit_waist
    test rbx, rbx
    je exit_waist
    cmp dword ptr [rbx+190h], 468F25F8h
    jne exit_waist
    jmp waist_base

waist_field:
    test dword ptr [light_flag], 1
    jne waist_off

    push rax
    mov eax, dword ptr [cfg_waist_light_brightness]
    mov dword ptr [rbx+144h], eax             ; 腰ライトの明るさ（フィールド）
    
    mov eax, dword ptr [cfg_waist_light_range]
    mov dword ptr [rbx+588h], eax             ; 腰ライトの照射範囲（フィールド）
    pop rax

    movss xmm3, dword ptr [rbx+144h]
    jmp exit_waist

waist_base:
    test dword ptr [light_flag], 1
    jne waist_off

    push rax
    mov eax, dword ptr [cfg_base_waist_brightness]
    mov dword ptr [rbx+144h], eax             ; 腰ライトの明るさ（拠点）
    
    mov eax, dword ptr [cfg_base_waist_range]
    mov dword ptr [rbx+588h], eax             ; 腰ライトの照射範囲（拠点）
    pop rax
    
    movss xmm3, dword ptr [rbx+144h]
    jmp exit_waist

waist_off:
    mov dword ptr [rbx+144h], 0
    xorps xmm3, xmm3

exit_waist:
    jmp qword ptr [g_WaistReturn]
WaistHook ENDP
END