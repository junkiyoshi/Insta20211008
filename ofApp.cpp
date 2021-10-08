#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(100, 3);
	this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->frame.clear();

	for (int x = -150; x <= 150; x += 20) {

		for (int z = -300; z <= 900; z += 20) {

			if (x >= -30 && x <= 30) {

				this->setBoxToMesh(this->face, this->frame, glm::vec3(x, -0.5, z), 1, 15, 15);
			}
			else {

				auto noise_value = ofNoise(x * 0.05, z * 0.005 + ofGetFrameNum() * 0.015);
				auto height = ofMap(noise_value, 0, 1.0, 0, -150);
				this->setBoxToMesh(this->face, this->frame, glm::vec3(x, height * -0.5, z), height, 15, 15);
			}
		}
	}

	for (int radius = 60; radius <= 100; radius += 15) {

		for (int i = 0; i < this->triangle_list.size(); i++) {

			glm::vec3 avg = (this->triangle_list[i].getVertex(0) + this->triangle_list[i].getVertex(1) + this->triangle_list[i].getVertex(2)) / 3;
			auto noise_value = ofNoise(glm::vec4(avg * 0.01, (radius / 20 + ofGetFrameNum()) * 0.008));

			if (noise_value > 0.65) {

				noise_value = 1;
			}
			else if (noise_value < 0.5) {

				noise_value = 0;
			}
			else {

				noise_value = ofMap(noise_value, 0.5, 0.65, 0, 1);
			}

			if (noise_value > 0) {

				vector<glm::vec3> vertices;

				vertices.push_back(glm::normalize((this->triangle_list[i].getVertex(0) - avg) * noise_value + avg) * (radius + 2));
				vertices.push_back(glm::normalize((this->triangle_list[i].getVertex(1) - avg) * noise_value + avg) * (radius + 2));
				vertices.push_back(glm::normalize((this->triangle_list[i].getVertex(2) - avg) * noise_value + avg) * (radius + 2));

				vertices.push_back(glm::normalize((this->triangle_list[i].getVertex(0) - avg) * noise_value + avg) * (radius - 2));
				vertices.push_back(glm::normalize((this->triangle_list[i].getVertex(1) - avg) * noise_value + avg) * (radius - 2));
				vertices.push_back(glm::normalize((this->triangle_list[i].getVertex(2) - avg) * noise_value + avg) * (radius - 2));

				for (auto& vertex : vertices) {

					vertex += glm::vec3(0, 300, 300);
				}

				this->face.addVertices(vertices);
				this->frame.addVertices(vertices);

				this->face.addTriangle(this->face.getNumVertices() - 1, this->face.getNumVertices() - 2, this->face.getNumVertices() - 3);
				this->face.addTriangle(this->face.getNumVertices() - 4, this->face.getNumVertices() - 5, this->face.getNumVertices() - 6);

				this->face.addTriangle(this->face.getNumVertices() - 1, this->face.getNumVertices() - 2, this->face.getNumVertices() - 5);
				this->face.addTriangle(this->face.getNumVertices() - 1, this->face.getNumVertices() - 5, this->face.getNumVertices() - 4);

				this->face.addTriangle(this->face.getNumVertices() - 1, this->face.getNumVertices() - 3, this->face.getNumVertices() - 6);
				this->face.addTriangle(this->face.getNumVertices() - 1, this->face.getNumVertices() - 6, this->face.getNumVertices() - 4);

				this->face.addTriangle(this->face.getNumVertices() - 2, this->face.getNumVertices() - 3, this->face.getNumVertices() - 6);
				this->face.addTriangle(this->face.getNumVertices() - 2, this->face.getNumVertices() - 6, this->face.getNumVertices() - 5);

				this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 2);
				this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 3);
				this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 3);

				this->frame.addIndex(this->frame.getNumVertices() - 4); this->frame.addIndex(this->frame.getNumVertices() - 5);
				this->frame.addIndex(this->frame.getNumVertices() - 5); this->frame.addIndex(this->frame.getNumVertices() - 6);
				this->frame.addIndex(this->frame.getNumVertices() - 4); this->frame.addIndex(this->frame.getNumVertices() - 6);

				this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 4);
				this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 5);
				this->frame.addIndex(this->frame.getNumVertices() - 3); this->frame.addIndex(this->frame.getNumVertices() - 6);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(180);

	ofSetColor(239);
	this->face.draw();

	ofSetColor(39);
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float size) {

	this->setBoxToMesh(face_target, frame_target, location, size, size, size);
}

//--------------------------------------------------------------
void ofApp::setBoxToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float height, float width, float depth) {

	int index = face_target.getVertices().size();

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * 0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * 0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * 0.5 * 0.99, depth * 0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * 0.5 * 0.99, depth * 0.5 * 0.99));

	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * -0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * -0.5 * 0.99, depth * -0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * 0.5 * 0.99, height * -0.5 * 0.99, depth * 0.5 * 0.99));
	face_target.addVertex(location + glm::vec3(width * -0.5 * 0.99, height * -0.5 * 0.99, depth * 0.5 * 0.99));

	face_target.addIndex(index + 0); face_target.addIndex(index + 1); face_target.addIndex(index + 2);
	face_target.addIndex(index + 0); face_target.addIndex(index + 2); face_target.addIndex(index + 3);

	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 6);
	face_target.addIndex(index + 4); face_target.addIndex(index + 6); face_target.addIndex(index + 7);

	face_target.addIndex(index + 0); face_target.addIndex(index + 4); face_target.addIndex(index + 1);
	face_target.addIndex(index + 4); face_target.addIndex(index + 5); face_target.addIndex(index + 1);

	face_target.addIndex(index + 1); face_target.addIndex(index + 5); face_target.addIndex(index + 6);
	face_target.addIndex(index + 6); face_target.addIndex(index + 2); face_target.addIndex(index + 1);

	face_target.addIndex(index + 2); face_target.addIndex(index + 6); face_target.addIndex(index + 7);
	face_target.addIndex(index + 7); face_target.addIndex(index + 3); face_target.addIndex(index + 2);

	face_target.addIndex(index + 3); face_target.addIndex(index + 7); face_target.addIndex(index + 4);
	face_target.addIndex(index + 4); face_target.addIndex(index + 0); face_target.addIndex(index + 3);

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * 0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * 0.5, depth * 0.5));

	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * -0.5));
	frame_target.addVertex(location + glm::vec3(width * 0.5, height * -0.5, depth * 0.5));
	frame_target.addVertex(location + glm::vec3(width * -0.5, height * -0.5, depth * 0.5));

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 1);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 2);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 3);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 0);

	frame_target.addIndex(index + 4); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 5); frame_target.addIndex(index + 6);
	frame_target.addIndex(index + 6); frame_target.addIndex(index + 7);
	frame_target.addIndex(index + 7); frame_target.addIndex(index + 4);

	frame_target.addIndex(index + 0); frame_target.addIndex(index + 4);
	frame_target.addIndex(index + 1); frame_target.addIndex(index + 5);
	frame_target.addIndex(index + 2); frame_target.addIndex(index + 6);
	frame_target.addIndex(index + 3); frame_target.addIndex(index + 7);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}