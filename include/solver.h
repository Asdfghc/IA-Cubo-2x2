#pragma once
#include <vector>
#include "cube_state.h"

using namespace std;

bool solve_bfs(const EstadoCodificado& estado_inicial, vector<Movimento>& caminho, int& n_visitados);
bool solve_dfs(const EstadoCodificado& estado_inicial, vector<Movimento>& caminho, int& n_visitados);
bool solve_astar(const EstadoCodificado& estado_inicial, vector<Movimento>& caminho, int& n_visitados, int ruido=0);
string solver_wrapper(const EstadoCodificado& estado_inicial, const string& metodo="astar", int ruido=0);
