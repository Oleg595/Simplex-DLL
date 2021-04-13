#pragma once
#include <iostream>
#include <vector>

struct __declspec(dllexport) Matrix {
private:
	struct Vector;
public:
	Matrix(size_t, size_t);
	Matrix(Matrix const&);

	Matrix& operator=(Matrix);
	Vector operator[](size_t);

	size_t get_n() const;
	size_t get_m() const;
	Matrix get_transposed() const;
	std::vector<double> gauss(std::vector<double>&);
	void Change_Str(size_t i, size_t j);//меняет местами строки i и j
	void Change_Col(size_t i, size_t j);//меняем местами столбцы i и j

	void print() const;

	~Matrix();
private:
	struct Vector {
		Vector(double*);
		double& operator[](size_t);
	private:
		double* vector;
	};

	double** matrix;
	size_t n, m;
};