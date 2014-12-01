package com.libiec61850.scl.model;

/*
 *  Copyright 2013, 2014 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */

public enum FunctionalConstraint {
	ST(0), /** Status information */
	MX(1), /** Measurands - analogue values */
	SP(2), /** Setpoint */
	SV(3), /** Substitution */
	CF(4), /** Configuration */
	DC(5), /** Description */
	SG(6), /** Setting group */
	SE(7), /** Setting group editable */
	SR(8), /** Service response / Service tracking */
	OR(9), /** Operate received */
	BL(10), /** Blocking */
	EX(11), /** Extended definition */
	CO(12), /** Control */
	ALL(99), /** All FCs */
	NONE(-1) /** not specified */
	;
	
	private int intValue;
	
	FunctionalConstraint(int intValue) {
		this.intValue = intValue;
	}
	
	public int getIntValue() {
		return this.intValue;
	}
	
	public static FunctionalConstraint createFromString(String fc) {
		return FunctionalConstraint.valueOf(fc);
	}
}
