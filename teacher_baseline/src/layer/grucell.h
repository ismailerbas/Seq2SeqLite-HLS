#ifndef GRU_CELL_H
#define GRU_CELL_H

void gru_cell(int input_size, int hidden_size, int input_features, float cell_hidden[1][hidden_size] , const float input[input_size][input_features], const float weights_x[][hidden_size*3],const  float weights_h[][hidden_size*3], const float* bias1, const float* bias2);

#endif  // GRU_CELL_H
