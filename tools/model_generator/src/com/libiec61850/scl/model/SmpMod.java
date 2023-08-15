package com.libiec61850.scl.model;

/*
 *  Copyright 2023 Michael Zillgith
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

public enum SmpMod
{
    SMP_PER_PERIOD(0),
    SMP_PER_SECOND(1),
    SEC_PER_SMP(2);

    private int value;

    private SmpMod(int value) {
        this.value = value;
    }

    public int getValue() {
        return value;
    }

    public boolean compare(int i)
    {
        return (value == i);
    }

    public static SmpMod fromValue(int val)
    {
        SmpMod[] errors = SmpMod.values();

        for (int i = 0; i < errors.length; i++) {
            if (errors[i].compare(val))
                return errors[i];
        }

        return SmpMod.SMP_PER_PERIOD;
    }
}
