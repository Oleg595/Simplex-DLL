#pragma once
#include <iostream>
#include <vector>
#include <limits>
#include"Simplex.h"
#include "Parser.h"
#include "Simplex.h"

struct SeidelMethod;
struct Limitations;
class Simplex;

#define N 5

struct Linear {
public:
	Linear(std::vector<double>&, Limitations&, std::vector<bool>&);

	Matrix get_Matrix();
	std::vector<double> get_b();
	std::vector<double> get_func();
	TT task_Type();

private:
	static size_t vars_in_canonical(std::vector<double>&, Limitations&, std::vector<bool>&);
	static bool next_combination(std::vector<size_t>&, size_t);

	Linear(std::vector<double>&, Limitations&, std::vector<bool>&, Linear*);
	void to_canonical(std::vector<double>&, Limitations&, std::vector<bool>&);
	void to_dual(std::vector<double>&, Limitations&, std::vector<bool>&);
	
	bool is_linear_independence(Matrix&);
	double calculate_objective(std::vector<double>&);
	Matrix sub_matrix(std::vector<size_t>&);
	double determinant(Matrix);
	bool in_allowable_area(std::vector<double>&);
	//double* ToDoubleArr(std::vector<double> vector);
	std::vector<double> back_to_original_vars(std::vector<double>&);

	size_t original_dimension;

	Matrix A;
	std::vector<double> b;
	std::vector<double> objective_function;
	TT task_type;

	std::vector<std::pair<size_t, size_t>> original_vars;

	Linear* dual_program;
};