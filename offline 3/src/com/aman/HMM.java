package com.aman;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

//Hidden MArkov model


import static java.lang.System.*;
class HM
{
	public static void main(String [] args)
	{
		int obs[]             = {3, 3, 2, 2, 1, 1, 3, 1, 3};
		int states[]           = {0, 1, 2, 3};
		double start_prob[]   = {1, 1, 1, 1};

		Forward obj = new Forward();
		double final_prob     = obj.compute(obs, states, start_prob, trans_prob, emiss_prob);
		out.println("The final probability for the given observation is  "+ final_prob);

		// Viterbi object creation

		Viterbi obj1 = new Viterbi();
		int final_path[] 	  = obj1.compute(obs, states, start_prob, trans_prob, emiss_prob);

		Backward obj2 = new Backward();

	}
}

import java.util.Scanner;
public class Forward
{


	public static double compute(int obs[], int states[], double start_prob[], double trans_prob[][], double emiss_prob[][]) throws ArrayIndexOutOfBoundsException
	{
		double forward[][] = new double[obs.length][states.length];

		out.println("The observation sequence - ");
		for(int i=0; i<obs.length; i++)
		{
			out.print(obs[i]+" ");
		}
		out.println();

		// Initializing the Forward Matrix
		for(int state : states)
		{
			forward[0][state] = start_prob[state] * emiss_prob[state][obs[0]-1];
		}

		for(int i=1; i<obs.length; i++)
		{
			for(int state1 : states)
			{
				forward[i][state1] = 0;

				for(int state2 : states)
				{
					forward[i][state1] += forward[i - 1][state2] * trans_prob[state2][state1];

				}
				forward[i][state1] *= emiss_prob[state1][obs[i] - 1];
			}
		}

		for(int i=0; i<obs.length; i++)
		{
			for(int j=0; j<states.length; j++)
			{
				out.println(forward[i][j]+"");
			}

		}

		double prob = 0;
		for(int state: states)
		{
			prob += forward[obs.length - 1][state];
		}


		return prob;
	}

}


public class Backward
{

	public static double compute(int obs[], int states[], double start_prob[], double trans_prob[][], double emiss_prob[][]) throws ArrayIndexOutOfBoundsException
	{
		double backward[][] = new double[states.length][obs.length];
		double prob = 0.0;

		out.println("The observation sequence - ");
		for(int i=0; i<obs.length; i++)
		{
			out.print(obs[i]+" ");
		}
		out.println();

		// Initializing the Backward Matrix

		for(int i=0;i<states.length;i++)
		{
			backward[i][obs.length -1]= 1;

		}

		for(int t = obs.length-2; t >= 0; t--)
		{

			for(int i = 0; i < states.length; i++)
			{

				backward[i][t] = 0;

				for(int j = 0; j < states.length; j++)
				{

					prob += (backward[j][t+1] * trans_prob[i][j] * emiss_prob[j][obs[t+1]]);

				}


			}
		}
		return prob;

	}
}
