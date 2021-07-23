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

import java.util.LinkedList;
import java.util.List;

import com.libiec61850.scl.DataAttributeDefinition;
import com.libiec61850.scl.DataObjectDefinition;
import com.libiec61850.scl.SclParserException;
import com.libiec61850.scl.types.DataAttributeType;
import com.libiec61850.scl.types.DataObjectType;
import com.libiec61850.scl.types.SclType;
import com.libiec61850.scl.types.TypeDeclarations;

public class DataObject implements DataModelNode {

    private String name;

    private int count;

    private List<DataAttribute> dataAttributes = null;
    private List<DataObject> subDataObjects = null;
    private SclType sclType;
    private DataModelNode parent;
    private boolean trans = false;

    public DataObject(DataObjectDefinition doDefinition, TypeDeclarations typeDeclarations, DataModelNode parent) throws SclParserException {
        this.name = doDefinition.getName();
        this.count = doDefinition.getCount();
        this.parent = parent;
        this.trans = doDefinition.isTransient();

        this.dataAttributes = new LinkedList<DataAttribute>();
        this.subDataObjects = new LinkedList<DataObject>();

        sclType = typeDeclarations.lookupType(doDefinition.getType(), DataObjectType.class);

        if (sclType == null)
            throw new SclParserException("type declaration missing for data object.");
        
        /* mark type as used */
        sclType.setUsed(true);

        createDataAttributes(typeDeclarations, sclType);

        createSubDataObjects(typeDeclarations, (DataObjectType) sclType);
    }

    private void createSubDataObjects(TypeDeclarations typeDeclarations, DataObjectType doType) throws SclParserException {

        List<DataObjectDefinition> sdoDefinitions = doType.getSubDataObjects();

        for (DataObjectDefinition sdoDefinition : sdoDefinitions) {
            this.subDataObjects.add(new DataObject(sdoDefinition, typeDeclarations, this));
        }
    }

    private void createDataAttributes(TypeDeclarations typeDeclarations, SclType sclType) throws SclParserException {

        List<DataAttributeDefinition> daDefinitions = null;

        if (sclType instanceof DataObjectType)
            daDefinitions = ((DataObjectType) sclType).getDataAttributes();

        if (sclType instanceof DataAttributeType)
            daDefinitions = ((DataAttributeType) sclType).getSubDataAttributes();

        for (DataAttributeDefinition daDefinition : daDefinitions) {
        	
        	if (daDefinition.getFc() == FunctionalConstraint.SE)
        	    this.dataAttributes.add(new DataAttribute(daDefinition, typeDeclarations, FunctionalConstraint.SG, this));
        	        	
            this.dataAttributes.add(new DataAttribute(daDefinition, typeDeclarations, null, this));
            

        }
    }

    public String getName() {
        return name;
    }

    public List<DataAttribute> getDataAttributes() {
        return dataAttributes;
    }

    public List<DataObject> getSubDataObjects() {
        return subDataObjects;
    }

    public int getCount() {
        return count;
    }
    
    public boolean isTransient() {
        return trans;
    }

    @Override
    public DataModelNode getChildByName(String childName) {
        for (DataAttribute dataAttribute : dataAttributes) {
            if (dataAttribute.getName().equals(childName))
                return dataAttribute;
        }

        for (DataObject dataObject : subDataObjects) {
            if (dataObject.getName().equals(childName))
                return dataObject;
        }

        return null;
    }

    @Override
    public SclType getSclType() {
        return sclType;
    }

    @Override
    public DataModelNode getParent() {
        return parent;
    }

}
