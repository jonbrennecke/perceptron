

params = struct( ...
	'inputs', 4, ...
	'outputs', 2, ...
	'activation', 'tanh', ...
	'initialization', 'random', ...
	'propogation', 'dotprod', ...
	'training', 'backPropogation' ... 
);

net = machine.Network( params );




	% auto params = machine::Network::Parameters();

	% params
	% 	.inputs(4)
	% 	.outputs(2)
	% 	.biasTerm(true)
	% 	.rate(0.001)
	% 	.activation( machine::hyperbolic_tan )
	% 	.initialization( machine::random )
	% 	.propogation( machine::dotprod )
	% 	.training( machine::backPropogation );

	% auto net = machine::Network( params );

	% std::vector<double> input = { 1.0, 2.0, 3.0, 4.0 };
	% std::vector<double> expected = { 1.0, 2.0 };

	% net.toggleTrainingMode();

	% net.train( input, expected );