#pragma once

#include <vector>
#include <iostream>
#include "Matrix.h"

enum TT {
	TT_MIN = 0,
	TT_MAX
};

class Simplex {
public:
	Simplex(Matrix&, std::vector<double>&, std::vector<double>&, TT type_task);
	double answer_func();
private:
	std::vector<std::pair<std::vector<double>, size_t>> data;//неравенства, в которых переменные, выражаются через другие переменные
	std::vector<double> func;//вектор, содержащий функцию
	std::vector<double> delta;//вектор delt
	double answer;//ответ
	bool have_ans;
	void Choose(size_t i, size_t min);
	size_t Determine(size_t i);
	size_t Check();
	size_t Num_Var(std::vector<double>&);
	bool Check_Data();
	void Positive_b();
};