
#include "Matrix.h"




//Size Constructor
Matrix::Matrix(const unsigned int rows, const unsigned int cols)
    : m_rows(rows)
    , m_cols(cols){

    if (rows == 0 || cols == 0){
        cerr << "Error: Matrix size constructor has 0 size" << endl;
        exit(1);
    }
    m_data = new double[rows * cols];
}

//Copy constructor
Matrix::Matrix(const Matrix& m)
    : m_rows(m.m_rows)
    , m_cols(m.m_cols){
    
    if (m_rows <= 0 || m_cols <= 0){
        cerr << "Error: Matrix copy constructor has 0 size" << endl;
        exit(1);
    }
    m_data = new double[m_rows * m_cols];
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            m_data[m_cols * row + col] = m.m_data[m.m_cols * row + col];
        }
    }
}

//Sets all matrix elements to 'n'
void Matrix::initialize(const int n){
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            m_data[m_cols * row + col] = n;
        }
    }
}

vector<double> Matrix::toVector() const{
    vector<double> temp;
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            temp.push_back(m_data[m_cols * row + col]);
        }
    }
    return temp;
}

unsigned int Matrix::numRows() const{
    return m_rows;
}

unsigned int Matrix::numCols() const{
    return m_cols;
}

//Prints the matrix as a grid to the console
void Matrix::printData() const{
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            printf("%9.4lf", m_data[m_cols * row + col]);
        }
        cout << endl;
    }
}

//Returns a copy of itself which is transposed
Matrix Matrix::transpose() const{
    Matrix temp(m_cols, m_rows);
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            temp(col, row) = m_data[m_cols * row + col];
        }
    }
    return temp;
}

//Destructor
Matrix::~Matrix(){
    delete[] m_data;
}

//Override = operator to copy each element
void Matrix::operator= (const Matrix& m){
    if(m.numRows() != m_rows || m.numCols() != m_cols){
        cerr << "Error: Matrix sizes are not equivalent. Cannot perform assignment." << endl;
        exit(1);
    }
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            m_data[m_cols * row + col] = m.m_data[m.m_cols * row + col];
        }
    }
}

//Override - (unary) operator to negate each element
Matrix Matrix::operator- () const{
    Matrix temp(m_rows, m_cols);
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            temp(row, col) = -1 * m_data[m_cols * row + col];
        }
    }
    return temp;
}

//Override + operator to add elementwise
Matrix Matrix::operator+ (const Matrix& a) const{
    if(a.numRows() != m_rows || a.numCols() != m_cols){
        cerr << "Error: Matrix sizes are not equivalent. Cannot perform addition." << endl;
        exit(1);
    }
    Matrix temp(m_rows, m_cols);
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            temp(row, col) = a.m_data[a.m_cols * row + col] + m_data[m_cols * row + col];
        }
    }
    return temp;
}

//Override - operator to subtract elementwise
Matrix Matrix::operator- (const Matrix& a) const{
    if(a.numRows() != m_rows || a.numCols() != m_cols){
        cerr << "Error: Matrix sizes are not equivalent. Cannot perform subtraction." << endl;
        exit(1);
    }
    Matrix temp(m_rows, m_cols);
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            temp(row, col) = m_data[m_cols * row + col] - a.m_data[a.m_cols * row + col];
        }
    }
    return temp;
}

//Override * operator to multiply elementwise by a constant
Matrix Matrix::operator* (const int a) const{
    Matrix temp(m_rows, m_cols);
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            temp(row, col) = a * m_data[m_cols * row + col];
        }
    }
    return temp;
}

//Override * operator to perform standard matrix multiplication
Matrix Matrix::operator* (const Matrix& a) const{
    if(m_cols != a.numRows()){
        cerr << "Error: Matrix sizes are incompatible. Cannot perform matrix multiplication." << endl;
        exit(1);
    }
    Matrix temp(m_rows, a.numCols());
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < a.numCols(); ++col){
            //perform cross product between current row and current column
            temp(row, col) = 0;
            for(unsigned int element = 0; element < m_cols; ++element){
                temp(row, col) += m_data[m_cols * row + element] * a(element, col);
            }
        }
    }
    return temp;
}

//Override () operator to allow access at a specific location
double& Matrix::operator() (const unsigned int row, const unsigned int col){
    if (row >= m_rows || col >= m_cols ){
        cerr << "Error: Matrix subscript out of bounds (too large)." << endl;
        exit(1);
    }
    return m_data[m_cols * row + col];
}

//Override () operator to allow access at a specific location
double Matrix::operator() (const unsigned int row, const unsigned int col) const{
    if (row >= m_rows || col >= m_cols){
        cerr << "Error: Matrix subscript out of bounds (too large)." << endl;
        exit(1);
    }
    return m_data[m_cols * row + col];
}

//Sets each element to a random double in the range [min, max] with specified resolution
void Matrix::initRand(const double min, const double max, const unsigned int resolution){
    if(min > max){
        cerr << "Error: initRand(): Min is larger than max." << endl;
        exit(1);
    }
    if(resolution == 0){
        cerr << "Error: initRand(): Resolution is equal to 0" << endl;
        exit(1);
    }
    for(unsigned int row = 0; row < m_rows; ++row){
        for(unsigned int col = 0; col < m_cols; ++col){
            /* resolution represents how many pieces each (int) is broken into.
             * bound is used because we can generate ints better than doubles.
             * bound is the product of the total range * resolution.
             * Afterward, we divide bound by resolution to get a double.
             */
            int bound = (int)( (double)resolution * (max - min) );
            //The double value is created by dividing bound by resolution
            double randDouble = min + (double)(rand() % bound) / resolution;
            //Passed into the matrix
            m_data[m_cols * row + col] = randDouble;
        }
    }
}

Matrix Matrix::addRowsCols(unsigned int extraRows, unsigned int extraCols, const double value){
    unsigned int newRows = m_rows + extraRows;
    unsigned int newCols = m_cols + extraCols;
    Matrix temp(newRows, newCols);
    
    for(unsigned int row = 0; row < newRows; ++row){
        for(unsigned int col = 0; col < newCols; ++col){
            if(row < m_rows && col < m_cols){
                temp(row, col) = m_data[m_cols * row + col];
            } else{
                temp(row, col) = value;
            }
        }
    }
    return temp;
}





