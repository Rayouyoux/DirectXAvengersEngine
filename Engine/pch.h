// pch.h : Il s'agit d'un fichier d'en-tête précompilé.
// Les fichiers listés ci-dessous sont compilés une seule fois, ce qui améliore les performances de génération des futures builds.
// Cela affecte également les performances d'IntelliSense, notamment la complétion du code et de nombreuses fonctionnalités de navigation du code.
// Toutefois, les fichiers listés ici sont TOUS recompilés si l'un d'entre eux est mis à jour entre les builds.
// N'ajoutez pas de fichiers fréquemment mis à jour ici, car cela annule les gains de performance.

#ifndef PCH_H
#define PCH_H

#include <windows.h>
#include <WindowsX.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3d12sdklayers.h>
#include <comdef.h>
#include <cassert>
#include <winerror.h>
#include <string>
#include <vector>
#include "Maths.h"
#include "D3DApp.h"
#include "framework.h"
#include "d3dx12.h"
#include "D3DUtils.h"
#include "GameTime.h"
#include "Entity.h"
#include "Transform.h"
#include "Component.h"

#endif //PCH_H
