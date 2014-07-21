%
%
%
%   _   _                                  _     _            
%  | |_| |__   ___    _ __ ___   __ _  ___| |__ (_)_ __   ___ 
%  | __| '_ \ / _ \  | '_ ` _ \ / _` |/ __| '_ \| | '_ \ / _ \
%  | |_| | | |  __/  | | | | | | (_| | (__| | | | | | | |  __/
%   \__|_| |_|\___|  |_| |_| |_|\__,_|\___|_| |_|_|_| |_|\___|
%                                                          
% 
%
% by @jonbrennecke / https://github.com/jonbrennecke
%
% Released under the MIT license (see the accompanying LICENSE.md)
%  
classdef Network

	properties (Hidden)
		handle;
	end

	properties (SetAccess = private)
    end
 
	methods

		% Constructor
		% 
		% :param params - 
		% 
		function this = Network( params )

			if ( exist('param','var') && isstruct(params) )
				% this.handle = 
				% constructor
			end

		end

	end
end