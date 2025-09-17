#pragma once
#include <vector>
#include "cube_state.h"

using namespace std;

// Retorna true se achou solução, false se não achou
// O caminho de movimentos é colocado em 'caminho' (vetor de chars)
bool solve_bfs(const EstadoCodificado& estado_inicial, vector<Movimento>& caminho);
bool solve_dfs(const EstadoCodificado& estado_inicial, vector<Movimento>& caminho);
bool solve_astar(const EstadoCodificado& estado_inicial, vector<Movimento>& caminho, int ruido);
