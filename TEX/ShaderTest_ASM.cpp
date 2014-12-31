//-----------------------------------------------------------------------------
//           Name: dx9_hlsl_simple_vs2ps.cpp
//         Author: Kevin Harris
//  Last Modified: 04/15/05
//    Description: This sample demonstrates how to write both a simple Vertex 
//                 and Pixel Shader using Direct3D's HLSL. The sample should be 
//                 considered a starting point or framework for more advanced 
//                 samples.
//
//   Control Keys: F1 - Toggle usage of vertex and pixel shaders.
//
// Note: The pixel shader has been changed slightly from what the 
//       fixed-function pipeline does by default so you can see a noticeable 
//       change when toggling the shaders on and off. Instead of modulating 
//       the vertex color with the texture's texel, the pixel shader adds the 
//       two together, which causes the pixel shader to produce a brighter, 
//       washed-out image. This modification can be switched back in the pixel 
//       shader file.
//-----------------------------------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <shellapi.h>
#include "resource.h"

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND                    g_hWnd          = NULL;
LPDIRECT3D9             g_pD3D          = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice    = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
LPDIRECT3DTEXTURE9      g_pTexture      = NULL;
LPDIRECT3DTEXTURE9      g_pTexture1      = NULL;
LPDIRECT3DCUBETEXTURE9      g_pTexture2      = NULL;

LPDIRECT3DVERTEXSHADER9      g_pVertexShader      = NULL;
LPDIRECT3DVERTEXDECLARATION9 g_pVertexDeclaration = NULL;
LPD3DXCONSTANTTABLE          g_pConstantTableVS   = NULL;

LPDIRECT3DPIXELSHADER9 g_pPixelShader      = NULL;
LPD3DXCONSTANTTABLE    g_pConstantTablePS  = NULL;

bool g_bUseShaders = true;

D3DXMATRIX g_matWorld;
D3DXMATRIX g_matView;
D3DXMATRIX g_matProj;
float      g_fSpinX = 0.0f;
float      g_fSpinY = 0.0f;

struct Vertex
{
    float x, y, z;
    DWORD color;
    float tu, tv;

    enum FVF
    {
        FVF_Flags = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
    };
};

Vertex g_quadVertices[] =
{
    {-1.0f, 1.0f, 0.0f,  0xffffff00,  0.0f,0.0f },
    { 1.0f, 1.0f, 0.0f,  0xff00ff00,  1.0f,0.0f },
    {-1.0f,-1.0f, 0.0f,  0xffff0000,  0.0f,1.0f },
    { 1.0f,-1.0f, 0.0f,  0xff0000ff,  1.0f,1.0f }
};
//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void init(void);
void render(void);
void shutDown(void);
void initShader(void);

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nCmdShow )
{
	WNDCLASSEX winClass; 
	MSG        uMsg;
    
   /* LPWSTR *szArgList;
    int argCount;
    szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);*/
    
    
    memset(&uMsg,0,sizeof(uMsg));

	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
    winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL, "MY_WINDOWS_CLASS", 
                             "Direct3D (DX9) - Simple Vertex & Pixel Shader Using HLSL",
                              WS_VISIBLE | WS_POPUP,
					         0, 0, 800, 600, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

	init();
	initShader();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else
		    render();
	}

	shutDown();

    UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							 UINT   msg, 
							 WPARAM wParam, 
							 LPARAM lParam )
{
	static POINT ptLastMousePosit;
	static POINT ptCurrentMousePosit;
	static bool bMousing;
	
    switch( msg )
	{
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

                case VK_F1:
                    g_bUseShaders = !g_bUseShaders;
                    break;
			}
		}
        break;

		case WM_LBUTTONDOWN:
		{

		}
		break;

		case WM_LBUTTONUP:
		{

		}
		break;

		case WM_MOUSEMOVE:
		{

		}
		break;

		case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}
		
        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;

		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void init( void )
{
    g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

    D3DDISPLAYMODE d3ddm;

    g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

    d3dpp.Windowed               = TRUE;
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat       = d3ddm.Format;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                          D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                          &d3dpp, &g_pd3dDevice );

	g_pd3dDevice->CreateVertexBuffer( 4*sizeof(Vertex), D3DUSAGE_WRITEONLY, 
                                      Vertex::FVF_Flags, D3DPOOL_DEFAULT, 
                                      &g_pVertexBuffer, NULL );
    void *pVertices = NULL;

    g_pVertexBuffer->Lock( 0, sizeof(g_quadVertices), (void**)&pVertices, 0 );
    memcpy( pVertices, g_quadVertices, sizeof(g_quadVertices) );
    g_pVertexBuffer->Unlock();

	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	D3DXMatrixPerspectiveFovLH( &g_matProj, D3DXToRadian( 45.0f ), 
                                800.0 / 600.0f, 0.1f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_matProj );

	D3DXMatrixIdentity( &g_matView ); // This sample is not really making use of a view matrix
}

//-----------------------------------------------------------------------------
// Name: initShader()
// Desc: Initialize a HLSL shader.
//-----------------------------------------------------------------------------
void initShader( void )
{
	//
	// Create a test texture for our effect to use...
	//
    LPDIRECT3DSURFACE9 surface, face_surface;
        D3DLOCKED_RECT locked_rect;

        g_pd3dDevice->CreateOffscreenPlainSurface( 128, 128,
                    D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, NULL);
	D3DXCreateTextureFromFile( g_pd3dDevice, "tex1.bmp", &g_pTexture );
    D3DXCreateTextureFromFile( g_pd3dDevice, "tex2.bmp", &g_pTexture1 );


   g_pd3dDevice->CreateCubeTexture( 128, 1, 0, D3DFMT_A8R8G8B8,
            D3DPOOL_DEFAULT, &g_pTexture2, NULL);


    surface->LockRect( &locked_rect, NULL, 0);

    int x,y,face;
    for (y = 0; y < 128; ++y)
    {
        DWORD *ptr = (DWORD *)(((BYTE *)locked_rect.pBits) + (y * locked_rect.Pitch));
        for (x = 0; x < 64; ++x)
        {
            *ptr++ = 0xff000000 | (( (x/2)%2== 0) ? 255:0) << 16;
        }
        for (x = 64; x < 128; ++x)
        {
            *ptr++ = 0xff000000 | (( (x/2)%2== 0) ? 255:0) << 16;
        }
    }

    surface->UnlockRect();

    for (face = 0; face < 6; ++face)
    {
        g_pTexture2->GetCubeMapSurface( (D3DCUBEMAP_FACES)face, 0, &face_surface);


        g_pd3dDevice->UpdateSurface( surface, NULL, face_surface, NULL);


        face_surface->Release();
    }
    //
    // Create a HLSL based vertex shader.
    //

    //
    // If your program uses explicit binding semantics (like this one), 
    // you can create a vertex declaration using those semantics.
    //

    D3DVERTEXELEMENT9 declaration[] =
    {
        { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
        { 0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };

    g_pd3dDevice->CreateVertexDeclaration( declaration, &g_pVertexDeclaration );

	HRESULT hr;
    LPD3DXBUFFER pCode;
    DWORD dwShaderFlags = 0;
	LPD3DXBUFFER pBufferErrors = NULL;

    // Assemble the vertex shader from the file
    hr = D3DXAssembleShaderFromFile( __argv[1], NULL, NULL, dwShaderFlags, &pCode,
                                    &pBufferErrors);



    if( FAILED(hr) )
	{
		LPVOID pCompilErrors = pBufferErrors->GetBufferPointer();
		MessageBox(NULL, (const char*)pCompilErrors, "Vertex Shader Compile Error",
			MB_OK|MB_ICONEXCLAMATION);
	}

    // Create the vertex shader
    g_pd3dDevice->CreateVertexShader( (DWORD*)pCode->GetBufferPointer(),
                                      &g_pVertexShader );
    pCode->Release();
    
    //
    // Create a HLSL based pixel shader.
    //

    // Assemble the vertex shader from the file
    hr = D3DXAssembleShaderFromFile( __argv[2], NULL, NULL, dwShaderFlags, &pCode,
                                    &pBufferErrors);

    if( FAILED(hr) )
	{
		LPVOID pCompilErrors = pBufferErrors->GetBufferPointer();
		MessageBox(NULL, (const char*)pCompilErrors, "Pixel Shader Compile Error",
			MB_OK|MB_ICONEXCLAMATION);
	}

    // Create the vertex shader
    g_pd3dDevice->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(),
                                      &g_pPixelShader );
    pCode->Release();
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
void shutDown( void )
{
	if( g_pTexture != NULL )
    {
		g_pTexture->Release(); 
        g_pTexture = NULL;
	}
	
    if( g_pVertexShader != NULL )
	{
		g_pVertexShader->Release(); 
        g_pVertexShader = NULL;
	}

    if( g_pConstantTableVS != NULL )
	{
		g_pConstantTableVS->Release(); 
        g_pConstantTableVS = NULL;
	}

    if( g_pVertexDeclaration != NULL )
	{
		g_pVertexDeclaration->Release(); 
        g_pVertexDeclaration = NULL;
	}

    if( g_pPixelShader != NULL )
	{
		g_pPixelShader->Release(); 
        g_pPixelShader = NULL;
	}

    if( g_pConstantTablePS != NULL )
	{
		g_pConstantTablePS->Release(); 
        g_pConstantTablePS = NULL;
	}

    if( g_pVertexBuffer != NULL ) 
    {
        g_pVertexBuffer->Release(); 
        g_pVertexBuffer = NULL; 
    }

    if( g_pd3dDevice != NULL ) 
    {
        g_pd3dDevice->Release(); 
        g_pd3dDevice = NULL; 
    }

    if( g_pD3D != NULL ) 
    {
        g_pD3D->Release(); 
        g_pD3D = NULL; 
    }
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
void render( void )
{
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(0.5f,0.5f,0.5f,1.0f), 1.0f, 0 );

	g_pd3dDevice->BeginScene();

    //
	// Set up a world matrix for spinning the quad about...
	//

	D3DXMATRIX matTrans;
	D3DXMATRIX matRot;

    D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, 4.0f );
	D3DXMatrixRotationYawPitchRoll( &matRot,
		                            D3DXToRadian(g_fSpinX),
		                            D3DXToRadian(g_fSpinY),
		                            0.0f );
    g_matWorld = matRot * matTrans;

    D3DXMatrixIdentity( &g_matView ); // This sample is not really making use of a view matrix
    
    if( g_bUseShaders == true )
    {
        //
        // Use vertex and pixel shaders...
        //

        D3DXMATRIX worldViewProjection = g_matWorld * g_matView * g_matProj;
       // g_pConstantTableVS->SetMatrix( g_pd3dDevice, "worldViewProj", &worldViewProjection );

        g_pd3dDevice->SetVertexDeclaration( g_pVertexDeclaration );
        g_pd3dDevice->SetVertexShader( g_pVertexShader );
        
/*
        //
        // NOTE:
        //
        // If our pixel shader had defined more than one sampler, we would 
        // need to look into our pixel shader's constants table and use the 
        // sampler's ASCII name to figure out which texture stage it had been 
        // mapped to on the hardware. The pixel shader we use here is so 
        // simple, we already know that are texture will end up on stage 0 so 
        // we can just call...
        //
        // g_pd3dDevice->SetTexture( 0, g_pTexture );
        //
        // and be done with it with out further worry.
        //

        D3DXHANDLE handle;

        if( handle = g_pConstantTablePS->GetConstantByName( NULL, "testTexture" ) )
        {
            D3DXCONSTANT_DESC constDesc;
            UINT count;

            g_pConstantTablePS->GetConstantDesc( handle, &constDesc, &count );

            if( constDesc.RegisterSet == D3DXRS_SAMPLER )
                g_pd3dDevice->SetTexture( constDesc.RegisterIndex, g_pTexture );
        }
//*/
        g_pd3dDevice->SetTexture( 0, g_pTexture );
        g_pd3dDevice->SetTexture( 1, g_pTexture1 );
        g_pd3dDevice->SetTexture( 3, g_pTexture2 );
        
        float constants[] = {0.02, 0, 0, -1.01, 0, -0.0266667, 0, 1.01333, 0, 0, 0, 1, 0, 0, 0, 1};
        
        g_pd3dDevice->SetPixelShader( g_pPixelShader );
        g_pd3dDevice->SetVertexShaderConstantF(0,static_cast<float*>(worldViewProjection), 4);
	    g_pd3dDevice->SetFVF( Vertex::FVF_Flags );
        g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0,sizeof(Vertex) );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

        g_pd3dDevice->SetVertexShader( NULL );
        g_pd3dDevice->SetPixelShader( NULL );
    }
    else
    {
        //
        // Render the normal way...
        //
    
        g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_matWorld );

        g_pd3dDevice->SetTexture( 0, g_pTexture );
	    g_pd3dDevice->SetFVF( Vertex::FVF_Flags );
        g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0,sizeof(Vertex) );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
    }
    
	g_pd3dDevice->EndScene();
	LPDIRECT3DSURFACE9 pd3dsBack = NULL;
	g_pd3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pd3dsBack)  ;
	D3DXSaveSurfaceToFileA(__argv[3], D3DXIFF_BMP,pd3dsBack, NULL, NULL);
	
	exit(0);
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}
