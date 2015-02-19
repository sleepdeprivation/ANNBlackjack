#include <fann.h>
#include <floatfann.h>
#include <fann_cpp.h>
using namespace std;
int main() {

	const unsigned int num_layers = 3;
	const unsigned int num_input = 11;
	const unsigned int num_hidden = 9;
	const unsigned int num_output = 1;

	//int arr [4]= {num_layers, num_input, num_hidden, num_hidden, num_hidden, num_hidden, num_hidden, num_hidden, num_hidden, num_hidden, num_output};

	FANN::neural_net net;
	//net.create_shortcut(2, num_input, num_output);
	net.create_standard(num_layers, num_input, num_hidden, num_output);
	//net.create_from_file("simpleTest.net");
	//net.set_training_algorithm( FANN::TRAIN_BATCH );

	net.set_activation_function_hidden( FANN::SIGMOID);
	net.set_activation_function_output( FANN::SIGMOID);
	//net.set_learning_rate(.001);

	//net.set_activation_function_

	//net.set_activation_function( FANN_SIGMOID );

	net.train_on_file("training.data", 200, 10, 0.008);
	//net.cascadetrain_on_file("training.data", 60, 4, .001);
	net.save("simpleTest.net");
	return 0;
}
