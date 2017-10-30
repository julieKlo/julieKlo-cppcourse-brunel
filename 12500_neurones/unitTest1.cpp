#include "Network.hpp"
#include <iostream>
#include "gtest/include/gtest/gtest.h"
#include "Constants.hpp"

TEST (NeuronTest, MembPotential) 
{
	Neuron n;
	n.setCurrElec(1.0);
	n.setCanSpike(true);
	
	n.update_state(0);
	
	EXPECT_EQ(tau*(1.0-exp(-tstart/tau)), n.getPotMemb());
	cout<<"			Right initial Membrane Potential"<<endl;
}

TEST (NeuronTest, MembPotInTime)
{
	Neuron n;
	n.setCurrElec(1.0);
	
	n.update_state(1);
	EXPECT_EQ(tau*(1.0-exp(-1/tau)), n.getPotMemb());
	cout<<"			Right time for membrane Potential"<<endl;
}

TEST (NeuronTest, NbSpikes)
{
	Neuron n;
	n.setCurrElec(1.0);
	
	n.update_state(1);
	EXPECT_EQ(0,n.getNbSpikes());
	
	int t(tstart);
	while(t<tstop)
	{
		n.update_state(t);
		++t;
	}
	EXPECT_NEAR(7,n.getNbSpikes(),3); //approximation car dépend de Cext
	cout<<"			Right Number of Spikes generated"<<endl;
	
	EXPECT_EQ(false,n.getTimes().empty());
	cout<<"		       Spike times have been stored and they are"<<endl;

	
}

TEST (NeuronTest, ConnexionGeneration)
{
	Neuron n1;
	n1.connexions_fill(1);
	EXPECT_EQ(conn_exc+conn_inh,n1.getNbConn());
	cout<<"			Connections generated"<<endl;
	
}

TEST (NetworkTest, ReceiveSignal)
{
	Neuron n1;
	Neuron n2;
	Network N(n1,n2);
	
	n1.setSpike(true);
	n1.emit_signal(N.getNeurons()[1],1);
	EXPECT_EQ(Je,N.getNeurons()[1]->getBufferDelay()[D]);
	cout<<"			Excitatory signal generated from one neuron to the other"<<endl;
	
	n1.setSpike(true);
	n1.setExcInhib(false);
	n1.emit_signal(N.getNeurons()[1],1);
	EXPECT_EQ(Ji,N.getNeurons()[1]->getBufferDelay()[D]);
	cout<<"			Inhibitory signal generated from one neuron to the other"<<endl;

 }					
 
TEST (NetworkTest, CreateNeurons)
{
	Network N;
	EXPECT_EQ(nbNeuronExc+nbNeuronIn,N.getNeurons().size());
	cout<<"			Neurons generated"<<endl;
}

TEST (NetworkTest, EnvironmentAttitude)
{
	Network N;
	EXPECT_EQ(nbNeuronExc+nbNeuronIn,N.getNeurons().size());
	cout<<"		    Right number of neurons"<<endl;
	
	int exc(0);
	int inh(0);
	for(size_t t(0);t<N.getNeurons().size();++t)
	{
		if(N.getNeurons()[t]->getExcInhib()) {exc+=1;}
		else{inh+=1;}
	}
	EXPECT_EQ(nbNeuronExc,exc);
	cout<<"		    Right number of excitatory neurons"<<endl;
	EXPECT_EQ(nbNeuronIn,inh);
	cout<<"		    Right number of inhibitory neurons"<<endl;
	
	
	int t(tstart);
	while(t<=t_refract+1)
	{
		N.update(t);
		++t;
		cout<<N.getNeurons()[0]->getPotMemb()<<endl;
	}
	for(size_t i(0);i<N.getNeurons().size();++i)
	{
	EXPECT_EQ(true,N.getNeurons()[i]->getPotMemb()!=0);
	}
	cout<<"		    All neurons evolve"<<endl;
	
	
	
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	
	return RUN_ALL_TESTS();
}
