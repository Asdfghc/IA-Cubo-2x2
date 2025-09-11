#pragma once
#include <vector>
#include "cube_state.h"

// Retorna true se achou solução, false se não achou
// O caminho de movimentos é colocado em 'caminho' (vetor de chars)
bool solve_bfs(const EstadoCodificado& estado_inicial, std::vector<char>& caminho);
