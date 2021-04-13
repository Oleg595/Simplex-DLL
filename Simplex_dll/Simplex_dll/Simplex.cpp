#include"Simplex.h"

Matrix::Matrix(size_t n, size_t m = 0) : n{ n }, m{ m == 0 ? n : m } {
	matrix = new double* [n];
	for (size_t i = 0; i < n; ++i) {
		matrix[i] = new double[m];
		for (size_t j = 0; j < m; ++j) {
			matrix[i][j] = 0;
		}
	}
}

Matrix::Matrix(Matrix const& other) : Matrix(other.n, other.m) {
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < m; ++j) {
			matrix[i][j] = other.matrix[i][j];
		}
	}
}

Matrix::Vector::Vector(double* vector) : vector{ vector } {}

Matrix& Matrix::operator=(Matrix other) {
	std::swap(matrix, other.matrix);
	std::swap(n, other.n);
	std::swap(m, other.m);
	return *this;
}

double& Matrix::Vector::operator[](size_t index) {
	return vector[index];
}

Matrix::Vector Matrix::operator[](size_t index) {
	return Vector(matrix[index]);
}

std::vector<double> Matrix::Gauss(std::vector<double> b) {
	for (size_t i = 0; i < n; i++) {
		size_t str;
		for (size_t j = 0; j < m; j++) {
			if (fabs(matrix[i][j]) > pow(10, -5)) {
				str = j;
				break;
			}
		}
		for (size_t j = 0; j < m; j++) {
			if (j != str)
				matrix[i][j] /= matrix[i][str];
		}
		b[i] /= matrix[i][str];
		matrix[i][str] = 1.;
		for (size_t j = 0; j < n; j++) {
			if (j != i) {
				for (size_t q = 0; q < m; q++) {
					if (q != str)
						matrix[j][q] -= matrix[j][str] * matrix[i][q];
				}
				b[j] -= matrix[j][str] * b[i];
				matrix[j][str] = 0.;
			}
		}
	}
	return b;
}

void Matrix::Change_Str(size_t i, size_t j) {
	for (size_t q = 0; q < m; q++) {
		double num = matrix[i][q];
		matrix[i][q] = matrix[j][q];
		matrix[j][q] = num;
	}
}

void Matrix::Change_Col(size_t i, size_t j) {
	for (size_t q = 0; q < n; q++) {
		double num = matrix[q][i];
		matrix[q][i] = matrix[q][j];
		matrix[q][j] = num;
	}
}

void Matrix::print() const {
	for (size_t i = 0; i < n; ++i) {
		for (size_t j = 0; j < m; ++j) {
			std::cout << matrix[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

size_t Matrix::get_n() const {
	return n;
}

size_t Matrix::get_m() const {
	return m;
}

Matrix Matrix::get_transposed() const {
	Matrix AT(m, n);
	for (size_t i = 0; i < m; ++i) {
		for (size_t j = 0; j < n; ++j) {
			AT[i][j] = matrix[j][i];
		}
	}
	return AT;
}

Matrix::~Matrix() {
	for (size_t i = 0; i < n; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

Simplex::Simplex(Matrix A, std::vector<double> st, std::vector<double> c, TT type_task) {
	have_ans = true;
	std::vector<double> b = A.Gauss(st);
	for (size_t i = 0; i < A.get_n(); i++) {
		std::vector<double> time_vector;
		time_vector.push_back(b[i]);
		for (size_t j = 0; j < A.get_m(); j++) {
			time_vector.push_back(A[i][j]);
		}
		data.push_back({ time_vector, Num_Var(time_vector) });
	}
	Positive_b();
	if (have_ans == false)
		return;
	size_t i = 0;
	answer = 0.;
	double coef = 1.;
	if (type_task == TT_MAX)
		coef = -1.;
	for (i = 0; i < c.size(); i++) {
		func.push_back(coef * c[i]);
	}
	for (; i < A.get_m(); i++) {
		func.push_back(0.);
	}
	size_t max = 0;
	int count = 0;
	while (((max = Check()) != func.size()) && (Check_Data())) {
		if (count < func.size()) {
			Choose(max + 1, Determine(max + 1));
			if (have_ans == false)
				return;
			count++;
		}
		else {
			size_t var = 0;
			size_t min;
			for (size_t i = 0; i < delta.size(); i++) {
				if (delta[i] > pow(10, -5)) {
					var = i;
					break;
				}
			}
			for (size_t i = 0; i < data.size(); i++) {
				if ((fabs(data[i].first[var + 1]) > pow(10, -5)) && (data[i].first[0] / data[i].first[var + 1] > pow(10, -5))) {
					min = i;
					break;
				}
			}
			for (size_t i = min + 1; i < data.size(); i++) {
				if ((fabs(data[i].first[var + 1]) > pow(10, -5)) && (data[0].first[var + 1] / data[i].first[var + 1] > pow(10, -5)) && (data[0].first[var + 1] / data[i].first[var + 1] > pow(10, -5))) {
					min = i;
				}
			}
			Choose(var + 1, min);
			if (have_ans == false)
				return;
			count = 0;
		}
	}
	if (type_task == TT_MAX) {
		for (size_t i = 0; i < func.size(); i++) {
			func[i] *= -1.;
		}
		answer *= -1.;
	}
}

size_t Simplex::Num_Var(std::vector<double> vector) {
	for (size_t i = 1; i < vector.size(); i++) {
		if (fabs(vector[i] - 1) < pow(10, -5))
			return i - 1;
	}
}

size_t Simplex::Determine(size_t i) {
	size_t min1 = 0;
	for (min1 = 0; min1 < data.size(); min1++) {
		if (data[min1].first[i] > pow(10, -5)) {
			break;
		}
	}
	std::cout << std::endl;
	for (size_t j = min1; j < data.size(); j++) {
		if (data[j].first[i] > pow(10, -5) && (data[j].first[0] / data[j].first[i] < data[min1].first[0] / data[min1].first[i])) {
			min1 = j; 
		}
		std::cout << data[j].first[0] / data[j].first[i] << std::endl;
	}
	return min1;
}

void Simplex::Positive_b() {
	int pos = -1;
	for (size_t i = 0; i < data.size(); i++) {
		if (data[i].first[0] < -pow(10, -5)) {
			pos = i;
			break;
		}
	}
	if (pos == -1)
		return;
	for (size_t i = pos + 1; i < data.size(); i++) {
		if ((data[i].first[0] < -pow(10, -5)) && (fabs(data[i].first[0]) > fabs(data[i].first[pos])))
			pos = i;
	}
	int var = -1;
	for (size_t i = 1; i < data[pos].first.size(); i++) {
		if ((data[pos].first[i] < -pow(10, -5)))
			var = i;
	}
	if (var == -1) {
		have_ans = false;
		return;
	}
	for (size_t i = var + 1; i < data[pos].first.size(); i++) {
		if ((data[pos].first[i] < -pow(10, -5)) && (fabs(data[pos].first[i]) > fabs(data[pos].first[var])))
			var = i;
	}
	Choose(var, pos);
}

size_t Simplex::Check() {
	delta.clear();
	for (size_t i = 0; i < func.size(); i++) {
		delta.push_back(-func[i]);
	}
	answer = 0;
	for (size_t i = 0; i < data.size(); i++) {
		size_t num = data[i].second;
		answer += func[num] * data[i].first[0];
		for (size_t j = 0; j < delta.size(); j++) {
			delta[j] += func[num] * data[i].first[j + 1]; 
		}
	}
	size_t max = 0;
	for (size_t i = 1; i < delta.size(); i++) {
		if (delta[max] < delta[i]) {
			max = i;
		}
	}
	if (delta[max] < pow(10, -5)) {
		return delta.size();
	}
	return max;
}

bool Simplex::Check_Data() {
	size_t check;
	for (size_t i = 0; i < data.size(); i++) {
		check = 1;
		for (size_t j = 1; j < data[i].first.size(); j++) {
			if ((fabs(data[i].first[j]) < pow(10, -5)) || (data[i].first[j] * data[i].first[0] < pow(10, -5)))
				check++;
			else
				break;
		}
		if (check == data[i].first.size()) {
			have_ans = false;
			return false;
		}
	}
	for (size_t i = 1; i < data[0].first.size(); i++) {
		check = 0;
		size_t num_null = 0;
		if (fabs(delta[i - 1]) < pow(10, -5))
			continue;
		for (size_t j = 0; j < data.size(); j++) {
			if ((fabs(data[j].first[i]) < pow(10, -5)) || (data[j].first[i] * delta[i - 1] < pow(10, -5)))
				check++;
			if (fabs(data[j].first[i]) < pow(10, -5)) {
				num_null++;
			}
			else
				break;
		}
		if ((check == data.size()) && (check != num_null)) {
			have_ans = false;
			return false;
		}
	}
	return true;
}

void Simplex::Choose(size_t i, size_t min1) {
	size_t min = min1;
	for (size_t j = 0; j < data[min].first.size(); j++) {
		if (j != i) {
			data[min].first[j] /= data[min].first[i];
		}
	}
	data[min].second = i - 1;
	data[min].first[i] = 1.;
	for (size_t j = 0; j < data.size(); j++) {
		if (j != min) {
			for (size_t q = 0; q < data[j].first.size(); q++) {
				if (q != i) {
					data[j].first[q] -= data[min].first[q] * data[j].first[i];
				}
			}
			data[j].first[i] = 0.;
		}
	}
	Positive_b();
}

void Simplex::Answer() {
	if (have_ans) {
		std::cout << "x[i]:" << std::endl;
		for (size_t i = 0; i < data[0].first.size(); i++) {
			std::cout << data[0].first[i] << ' ';
		}
		std::cout << std::endl;
		std::cout << "F = " << answer << std::endl;
	}
	else
		std::cout << "No answer" << std::endl;
}