/**
 * Copyright 2011 Joao Miguel Pereira
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package eu.jpereira.trainings.designpatterns.structural.composite.model;

import java.util.ArrayList;
import java.util.List;


/**
 * @author Joao Pereira
 * 
 */
public abstract class CompositeShape extends Shape {

	public List<Shape> shapes;
	public List<Shape> shapesByType;
	public List<Shape> leafShapes;

	public CompositeShape() {
		this.shapes = createShapesList();
	}

	public CompositeShape asComposite() {
		//TODO: Implement
		return this;
	}
	/**
	 * Remove a shape from this shape children
	 * 
	 * @param shape
	 *            the shape to remove
	 * @return true if the shape was present and was removed, false if the shape
	 *         was not present
	 */
	public boolean removeShape(Shape shape) {
		// TODO: implement
		if(shapes.remove(shape)) {
			return true;
		}
		else {
			return false;
		}
	}

	/**
	 * Return the total shapes count in case this is a composite
	 * 
	 * @return the total count of shapes if the shape is composite. -1 otherwise
	 */
	public int getShapeCount() {
		// TODO: implement
		return shapes.size();

	}

	/**
	 * Add a shape to this shape.
	 * 
	 * @param shape
	 *            The shape to add
	 * @throws ShapeDoesNotSupportChildren
	 *             if this shape is not a composite
	 */
	public void addShape(Shape shape) {
		// TODO: Implement		
		shapes.add(shape);
	}

	public List<Shape> getShapes() {
		// TODO: Implement
		return shapes;

	}

	/**
	 * @param shapeType
	 * @return
	 */
	
	public List<Shape> getShapesByType(ShapeType shapeType) {
		// TODO: Implement
		int index = 0;
		this.shapesByType = new ArrayList<Shape>();
		
		while (index < shapes.size()) {
			if (shapes.get(index).getType().equals(shapeType)) {
				shapesByType.add(shapes.get(index));
			}
			index++;
		}

		return shapesByType;
	}

	/**
	 * Return all shapes that are leafs in the tree
	 * 
	 * @return
	 */
	public List<Shape> getLeafShapes() {
		// TODO: Implement
		int index = 0;
		this.leafShapes = new ArrayList<Shape>();
		
		while (index < shapes.size()) {
			if (shapes.get(index).getType().equals(LeafShape.class)) {
				leafShapes.add(shapes.get(index));
			}
			index++;
		}
		return leafShapes;
	}

	/**
	 * Factory method for the list of children of this shape
	 * 
	 * @return
	 */
	protected List<Shape> createShapesList() {
		//List is an interface. Interfaces cannot be instantiated.
		shapes = new ArrayList<Shape>();
		// TODO: Implement
		return shapes;
	}
}
