#pragma once

#ifdef SIMPLEX_H_EXPORTS
#define SIMPLEX_H __declspec(dllexport)
#else
#define SIMPLEX_H __declspec(dllimport)
#endif

#include <vector>
#include <iostream>

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
	std::vector<double> Gauss(std::vector<double>*);
	void Change_Str(size_t i, size_t j);//������ ������� ������ i � j
	void Change_Col(size_t i, size_t j);//������ ������� ������� i � j

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

enum __declspec(dllexport) TT {
	TT_MIN = 0,
	TT_MAX
};

class __declspec(dllexport) Simplex {
public:
	Simplex(Matrix*, std::vector<double>*, std::vector<double>*, TT type_task);
	double Answer();
private:
	std::vector<std::pair<std::vector<double>, size_t>> data;//�����������, � ������� ����������, ���������� ����� ������ ����������
	std::vector<double> func;//������, ���������� �������
	std::vector<double> delta;//������ delt
	double answer;//�����
	bool have_ans;
	void Choose(size_t i, size_t min);
	size_t Determine(size_t i);
	size_t Check();
	size_t Num_Var(std::vector<double>*);
	bool Check_Data();
	void Positive_b();
};