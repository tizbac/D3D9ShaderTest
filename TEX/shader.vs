//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
// Parameters:
//
//   float4x4 worldViewProj;
//
//
// Registers:
//
//   Name          Reg   Size
//   ------------- ----- ----
//   worldViewProj c0       4
//

    vs_3_0
    def c4, 1, 0, 0, 0
    dcl_position v0
    dcl_color v1
    dcl_texcoord v2
    dcl_position o0
    dcl_color o1
    dcl_texcoord1 o2.xy
    dcl_texcoord2 o3.xy
    dcl_texcoord3 o4.xy
    dcl_texcoord4 o5.xyz
    mad r0, v0.xyzx, c4.xxxy, c4.yyyx
    dp4 o0.x, r0, c0
    dp4 o0.y, r0, c1
    dp4 o0.z, r0, c2
    dp4 o0.w, r0, c3
    mov o1, v1
    mov o2.xy, v1
    mov o3.xy, v0
    mov o4.xy, v2
    mov o5.xyz, v1
// approximately 7 instruction slots used
