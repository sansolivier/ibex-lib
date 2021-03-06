//============================================================================
//                                  I B E X                                   
// File        : ibex_CtcART.cpp
// Author      : Ignacio Araya, Bertrand Neveu , Gilles Trombettoni
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 1, 2012
// Last Update : Nov 15, 2012
//============================================================================

#include "ibex_LinearRelaxAffine2.h"
#include "ibex_EmptyBoxException.h"

namespace ibex {

// the constructor
LinearRelaxAffine2::LinearRelaxAffine2(const System& sys1) :
				LinearRelax(sys1), sys(sys1) {

}

LinearRelaxAffine2::~LinearRelaxAffine2() {

}


/*********generation of the linearized system*********/
int LinearRelaxAffine2::linearization(const IntervalVector& box, LinearSolver& lp_solver) {

	Affine2 af2;
	Vector rowconst(sys.nb_var);
	Interval ev(0.0);
	Interval center(0.0);
	Interval err(0.0);
	CmpOp op;
	int cont = 0;
	LinearSolver::Status stat = LinearSolver::FAIL;


	// Create the linear relaxation of each constraint
	for (int ctr = 0; ctr < sys.nb_ctr; ctr++) {

		af2 = 0.0;
		ev = sys.ctrs[ctr].f.eval_affine2(box, af2);
		op = sys.ctrs[ctr].op;

		if (af2.size() == sys.nb_var) { // if the affine2 form is valid

			// convert the epsilon variables to the original box
			double tmp=0;
			center =0;
			err =0;
			for (int i =0; i <sys.nb_var; i++) {
				tmp = box[i].rad();
				//		if (tmp> lp_solver.getEpsilon()) {
				rowconst[i] =af2.val(i+1) / tmp;
				center += rowconst[i]*box[i].mid();
				err += fabs(rowconst[i])*  pow(2,-50); // TODO to check
				//		} else {
				//			rowconst[i] = 0;
				//			err += tmp;
				//		}
			}

			switch (op) {
			case LEQ:
				if (0.0 == ev.lb())
					throw EmptyBoxException();
			case LT: {
				if (0.0 < ev.lb())
					throw EmptyBoxException();
				else if (0.0 < ev.ub()) {
					stat = lp_solver.addConstraint(rowconst, LEQ,	((af2.err()+err) - (af2.val(0)-center)).ub());
					if (stat == LinearSolver::OK)	cont++;
				}
				break;
			}
			case GEQ:
				if (ev.ub() == 0.0)
					throw EmptyBoxException();
				break;
			case GT: {
				if (ev.ub() < 0.0)
					throw EmptyBoxException();
				else if (ev.lb() < 0.0) {
					stat = lp_solver.addConstraint(rowconst, GEQ,	(-(af2.err()+err) - (af2.val(0)-center)).lb());
					if (stat == LinearSolver::OK)	cont++;
				}
				break;
			}
			case EQ: {
				if (!ev.contains(0.0)) {
					throw EmptyBoxException();
				}
				else {
					if (ev.diam()>2*lp_solver.getEpsilon()) {
						stat = lp_solver.addConstraint(rowconst, GEQ,	(-(af2.err()+err) - (af2.val(0)-center)).lb());
						if (stat == LinearSolver::OK)	cont++;
						stat = lp_solver.addConstraint(rowconst, LEQ,	((af2.err()+err) - (af2.val(0)-center)).ub());
						if (stat == LinearSolver::OK)	cont++;
					}
				}
				break;
			}
			}
		}

	}
	return cont;

}

//void CtcART::convert_back(IntervalVector & box, IntervalVector & epsilon) {
//
//	for (int i = 0; i < box.size(); i++) {
//		box[i] &= box[i].mid() + (box[i].rad() * epsilon[i]);
//	}
//}

}
