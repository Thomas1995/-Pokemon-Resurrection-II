#include "stdafx.h"
#include "Trainer.h"

Trainer::Trainer(const std::string trainerName) 
	: name(trainerName) {}

MainTrainer::MainTrainer(const std::string trainerName) 
	: Trainer(trainerName) {}
