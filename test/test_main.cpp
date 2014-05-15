/*
 * test_main.cpp
 *
 *   Author: Ying Ni(yni6@hawk.iit.edu)
 *   Description:
 *
 */

#include "log/logger.h"
#include <time.h>
#include "model/object.h"
#include "model/property.h"
#include "model/relation.h"
#include "model/types.h"
#include "model/prop_value.h"
#include "model/node.h"
#include "common.h"
#include "helper/helper.h"
#include "manager/manager.h"
#include "wrapper/zht_wrapper.h"
#include <iostream>
#include <stdio.h>
#include "zht/cpp_zhtclient.h"
#include <stdlib.h>
using namespace std;
using namespace graphdb;

static void testLogger();
static void testProperty();
static void testPropertyValue();
static void testRelation();
static void testNode();
static void testRemoveNode();
static void testRemoveRelation();
static void benchmarkInsertZHT();
static void benchmarkLookupZHT();
static void benchmarkRemoveZHT();
static void benchmarkCreateNode();
static void benchmarkCreateRelation();
static void benchmarkLookupNode();
static void benchmarkLookupRelation();
static void benchmarkRemoveNode();
static void benchmarkRemoveRelation();
static void benchmarkAddPropertyToOneNode();
static void benchmarkAddPropertyToManyNode();
static void benchmarkCreateStarGraph();
static void benchmarkTravelStarGraph();
static void benchmarkRemoveStarGraph();
static void benchmarkCreateLinkedListGraph();
static void benchmarkTravelLinkedListGraph();
static void	benchmarkRemoveLinkedListGraph();
static void benchmarkCreateTwitterGraph();
static void benchmarkTwitterUnfollow(long int follower,long int followee);
static void testDone();

static int count = 10000;

int main(void) {

	testLogger();
	testProperty();
	testPropertyValue();
	testRelation();
	testNode();
	testRemoveNode();
	testRemoveRelation();

	benchmarkInsertZHT();
	benchmarkLookupZHT();
	benchmarkRemoveZHT();

	benchmarkCreateNode();
	benchmarkCreateRelation();
	benchmarkLookupNode();
	benchmarkLookupRelation();
	benchmarkRemoveNode();
	benchmarkRemoveRelation();

	benchmarkAddPropertyToOneNode();
	benchmarkAddPropertyToManyNode();

	benchmarkCreateStarGraph();
	benchmarkTravelStarGraph();
//	benchmarkRemoveStarGraph();

	benchmarkCreateLinkedListGraph();
	benchmarkTravelLinkedListGraph();
	benchmarkRemoveLinkedListGraph();

	//TODO benchmark more complicated situations
	//read real twitter data.

	testDone();

	return 0;
}

static void testLogger()
{
	initLogger(stdout, INFO);

	LOG_FATAL("This is %s message, level is %d.", "fatal", FATAL);
	LOG_ERROR("This is %s message, level is %d.", "error", ERROR);
	LOG_WARN("This is %s message, level is %d.", "warn", WARN);
	LOG_INFO("This is %s message, level is %d.", "info", INFO);
	LOG_DEBUG("This is %s message, level is %d.", "debug", DEBUG);
	LOG_TRACE("This is %s message, level is %d.", "trace", TRACE);

	cout << "testLogger Pass" << endl;
}

static void testProperty()
{
	Property *o1 = new Property("test", DT_STRING);
	Object *o2 = new Property("test", DT_STRING);
	Object *o3 = new Object();
	Object *o4 = o3;
	cout << o1 << " " << o2 << " " << o3 << endl;
	cout << instanceOf<Relation>(*o2) << instanceOf<Property>(*o2) << endl;
	cout << (*o1 == *o2) << (*o2 == *o1) << (*o2 == *o3) << (*o4 == *o3) << endl;

	cout << o1->toString() << endl;

	destroy(o1);
	destroy(o2);
	destroy(o3);

	cout << "testProperty Pass" << endl;
}

static void testPropertyValue()
{
	String s("abc");
	String ss = string("abc");
	Integer i(1);
	Integer ii = 1;
	Double d(2.2);
	Double dd = 2.2;
	Boolean b(true);
	Boolean bb = false;
	Byte byte('x');
	Byte bytebyte = 'x';

	cout << s.toString() << i.toString() << d.toString() << b.toString() << byte.toString() << endl;
	cout << ss.toString()<< ii.toString() << dd.toString() << bb.toString() << bytebyte.toString() << endl;

	cout << "Test PropertyValue Pass" << endl;
}

static void testRelation()
{
	Manager *manager = Manager::getInstance();
	Relation *rel = manager->createRelation(0, "knows");

	rel->setStartNode(Node::getUUID(0));
	rel->setEndNode(Node::getUUID(1));

	rel->addProperty("age", DT_INT, new Integer(25));
	rel->addProperty("firstName", DT_STRING, new String("zephyring"));

	cout << "Has property age: " << rel->hasProperty("age", DT_INT) << endl;
	cout << "Has property firstName: " << rel->hasProperty("firstName", DT_STRING) << endl;
	manager->commit(rel);//save into zht and free all memory about this relation

	manager->printRelation(0);
	cout << "Test Relation Pass" << endl;
}

static void testNode()
{
	Manager *manager = Manager::getInstance();

	Node *node = manager->createNode(0);
	Node *node1 = manager->createNode(1);
	Relation *rel = manager->createRelation(0, "knows");
	Relation *rel1 = manager->createRelation(1, "likes");
	Relation *rel2 = manager->createRelation(2,"knows");

	node->addRelationTo(*node1, *rel);
	node->addRelationTo(*node1, *rel1);
	node->addRelationTo(*node1, *rel2);

	node->addProperty("age", DT_INT, new Integer(24));
	node->addProperty("firstName", DT_STRING, new String("zephyr"));

	manager->commit(node);//save into zht
	manager->commit(node1);
	manager->commit(rel);
	manager->commit(rel1);
	manager->commit(rel2);

	manager->printNode(0);
	manager->printNode(1);

	cout << "Test Node Pass" << endl;
}

static void testRemoveNode()
{
	Manager *manager = Manager::getInstance();

	Node *node = manager->createNode(0);
	Node *node1 = manager->createNode(1);
	Relation *rel = manager->createRelation(0, "knows");
	Relation *rel1 = manager->createRelation(1, "likes");
	Relation *rel2 = manager->createRelation(2,"knows");

	node->addRelationTo(*node1, *rel);
	node->addRelationTo(*node1, *rel1);
	node->addRelationTo(*node1, *rel2);

	node->addProperty("age", DT_INT, new Integer(24));
	node->addProperty("firstName", DT_STRING, new String("zephyr"));

	cout << "Before commit, Node: " << node->toString() << endl;

	manager->commit(node);//save into zht
	manager->commit(node1);
	manager->commit(rel);
	manager->commit(rel1);
	manager->commit(rel2);

	node = manager->getNode(0);
	cout << "After commit, Node: " << node->toString() << endl;
	cout << "Before remove, Node: " << node->toString() << endl;
	destroy(node);

	manager->removeNode(1);
	node = manager->getNode(0);
	cout << "After remove, Node: " << node->toString() << endl;
	destroy(node);
	cout << "Test Remove Node Pass" << endl;
}

static void testRemoveRelation(){

	Manager *manager = Manager::getInstance();
	//four nodes
	Node *node = manager->createNode(0);
	Node *node1 = manager->createNode(1);
	Node *node2 = manager->createNode(2);
	Node *node3 = manager->createNode(3);

	//four relations
	Relation *rel = manager->createRelation(0,"knows");
	Relation *rel1 = manager->createRelation(1,"like");
	Relation *rel2 = manager->createRelation(2,"knows");

	//construct a graph
	node->addRelationTo(*node1,*rel);
	node->addRelationTo(*node2,*rel1);
	node->addRelationTo(*node3,*rel2);

	//add property
	node->addProperty("namea",DT_STRING,new String("bita"));
	node1->addProperty("nameb",DT_STRING,new String("bitb"));
	node2->addProperty("namec",DT_STRING,new String("bitc"));
	node3->addProperty("named",DT_STRING,new String("bitd"));
	cout<<"Before commit, Relation1: "<<rel->toString()<<endl;
	cout<<"Before commit, Relation2: "<<rel1->toString()<<endl;
	cout<<"Before commit, Relation3: "<<rel2->toString()<<endl;

	//commit
	manager->commit(node);
	manager->commit(node1);
	manager->commit(node2);
	manager->commit(node3);
	manager->commit(rel);
	manager->commit(rel1);
	manager->commit(rel2);

	rel = manager->getRelation(0);
	cout<<"After commit, Relation1: "<<rel->toString()<<endl;

	manager->removeRelation(0);
	manager->removeRelation(1);
	manager->removeRelation(2);
	destroy(rel);
	cout<<"Test remove relation pass"<<endl;

}

static void benchmarkInsertZHT()
{
	ZHT *zht = ZHT::getInstance();
	string key = "key";
	string value = "value";

	clock_t start = clock();
	for(int i = 0; i < count; i++)
	{
		stringstream k;
		k << key << i;
		string kk = k.str();
		zht->insertKV(kk, value);
	}
	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Insert " << count << " KV pairs cost: " << diff << " seconds" << endl;
}

static void benchmarkLookupZHT()
{
	ZHT *zht = ZHT::getInstance();
	string key = "key";
	string value;

	clock_t start = clock();
	for(int i = 0; i < count; i++)
	{
		stringstream k;
		k << key << i;
		string kk = k.str();
		zht->lookupKV(kk, value);
	}
	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Lookup " << count << " KV pairs cost: " << diff << " seconds" << endl;
}

static void benchmarkRemoveZHT()
{
	ZHT *zht = ZHT::getInstance();
	string key = "key";

	clock_t start = clock();
	for(int i = 0; i < count; i++)
	{
		stringstream k;
		k << key << i;
		string kk = k.str();
		zht->removeKV(kk);
	}
	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Remove " << count << " KV pairs cost: " << diff << " seconds" << endl;
}

static void benchmarkCreateNode()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	for(int i = 0; i < count; i++)
	{
		Node *node = manager->createNode(i);
		manager->commit(node);
	}

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Create " << count << " nodes cost: " << diff << " seconds" << endl;
}

static void benchmarkCreateRelation()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	for(int i = 0; i < count; i++)
	{
		Relation *rel = manager->createRelation(i,"knows");
		manager->commit(rel);
	}

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Create " << count << " relations cost: " << diff << " seconds" << endl;
}

static void benchmarkLookupNode()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	for(int i = 0; i < count; i++)
	{
		Node *node = manager->getNode(i);
		destroy(node);
	}

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Lookup " << count << " nodes cost: " << diff << " seconds" << endl;
}

static void benchmarkLookupRelation()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	for(int i = 0; i < count; i++)
	{
		Relation *rel = manager->getRelation(i);
		destroy(rel);
	}

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Lookup " << count << " relations cost: " << diff << " seconds" << endl;
}

static void benchmarkRemoveNode()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	for(int i = 0; i < count; i++)
	{
		manager->removeNode(i);
	}

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Remove " << count << " nodes cost: " << diff << " seconds" << endl;
}

static void benchmarkRemoveRelation()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	for(int i = 0; i < count; i++)
	{
		manager->removeRelation(i);
	}

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Remove " << count << " relations cost: " << diff << " seconds" << endl;
}

static void benchmarkAddPropertyToOneNode()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	Node *node = manager->createNode(0);

	for(int i = 0; i < count; i++)
	{
		stringstream name;
		name << "prop" << i;
		node->addProperty(name.str(), DT_INT, new Integer(i));
	}

	manager->commit(node);
	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Add " << count << " properties to 1 node cost: " << diff << " seconds" << endl;
}

static void benchmarkAddPropertyToManyNode()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	for(int i = 0; i < count / 100; i++)
	{
		Node *node = manager->createNode(i);
		for(int j = 0; j < count / 100; j++)
		{
			stringstream name;
			name << "prop" << j;
			node->addProperty(name.str(), DT_INT, new Integer(j));
		}
		manager->commit(node);
	}

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Add " << count / 100 << " properties to " << count / 100 << " nodes cost: " << diff << " seconds" << endl;
}

static void benchmarkCreateStarGraph()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	Node *node = manager->createNode(0);

	for(int i = 1; i < count; i++)
	{
		Node *node1 = manager->createNode(i);
		Relation *rel = manager->createRelation(i, "knows");
		node->addRelationTo(*node1, *rel);
		manager->commit(node1);
		manager->commit(rel);
	}

	manager->commit(node);

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Create Star Graph of " << count << " nodes cost: " << diff << " seconds" << endl;
}

static void benchmarkTravelStarGraph()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	Node *node = manager->getNode(0);

	set<string> rels = node->getRelations("knows", OUT);

	for(set<string>::iterator it = rels.begin(); it != rels.end(); it++)
	{
		Relation *rel = manager->getRelation(Relation::getId(*it));
		Node *node1 = manager->getNode(Node::getId(rel->getEndNode()));
		destroy(rel);
		destroy(node1);
	}

	destroy(node);

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Traversal Star Graph of " << count << " nodes cost: " << diff << " seconds" << endl;
}

static void benchmarkRemoveStarGraph()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	for(int i = 0; i < count; i++)
		manager->removeNode(i);

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Remove Star Graph of " << count << " nodes cost: " << diff << " seconds" << endl;
}

static void benchmarkCreateLinkedListGraph()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	vector<Node*> v;

	for(int i = 0; i < count; i++)
		v.push_back(manager->createNode(i));

	for(int i = 0; i < count - 1; i++)
	{
		Relation *r = manager->createRelation(i, "knows");
		v[i]->addRelationTo(*v[i+1], *r);
		manager->commit(r);
	}

	for(int i = 0; i < count; i++)
		manager->commit(v[i]);

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Create LinkedList Graph of " << count << " nodes cost: " << diff << " seconds" << endl;
}

static void benchmarkTravelLinkedListGraph()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	Node *node = manager->getNode(0);

	while(true)
	{
		set<string> rels = node->getRelations("knows", OUT);

		//if reach the end of the linked list, stop
		if(rels.empty())
			break;

		for(set<string>::iterator it = rels.begin(); it != rels.end(); it++)
		{
			Relation *rel = manager->getRelation(Relation::getId(*it));
			Node *newNode = manager->getNode(Node::getId(rel->getEndNode()));
			destroy(node);
			node = newNode;
			destroy(rel);
		}
	}

	destroy(node);

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Traversal LinkedList Graph of " << count << " nodes cost: " << diff << " seconds" << endl;
}

static void benchmarkRemoveLinkedListGraph()
{
	Manager *manager = Manager::getInstance();

	clock_t start = clock();

	for(int i = 0; i < count; i++)
		manager->removeNode(i);

	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout << "Remove LinkedList Graph of " << count << " nodes cost: " << diff << " seconds" << endl;
}

static void benchmarkCreateTwitterGraph(){
	Manager *manager = Manager::getInstance();
	long int follower;
	long int followee;
	clock_t start = clock();
	long int i = 0;

	//get file data here
	FILE* t_file = fopen("./twitter_rv.net","r");
	if(t_file == NULL){
		cout<<"Cannot open file."<<endl;
		exit(0);
	}

	while(fscanf(t_file,"%d	%d\n",&follower,&followee) != EOF){

		//manager commit to ZHT database. Property: "follow", from followee to follower.
		if((manager->getNode(follower) == NULL)&&(manager->getNode(followee == NULL))){

			Node* node1 = manager->createNode(follower);
			Node* node2 = manager->createNode(followee);
			Relation* rel = manager->createRelation(i,"follow");
			node1->addRelationTo(*node2,*rel);
			cout<<"Follow complete. From "<<follower<<" to "<<followee<<endl;

			manager->commit(node1);
			manager->commit(node2);
			manager->commit(rel);
			i++;
			destroy(node1);
			destroy(node2);
			destroy(rel);

		}else if((manager->getNode(follower) == NULL)&&(manager->getNode(followee) != NULL)){

			Node* node1 = manager->createNode(follower);
			Node* node2 = manager->getNode(followee);
			Relation* rel = manager->createRelation(i,"follow");
			node1->addRelationTo(*node2,*rel);
			cout<<"Follow complete. From "<<follower<<" to "<<followee<<endl;

			manager->commit(node1);
			manager->commit(node2);
			manager->commit(rel);
			i++;
			destroy(node1);
			destroy(node2);
			destroy(rel);

		}else if((manager->getNode(follower) != NULL)&&(manager->getNode(followee) == NULL)){

			Node* node1 = manager->createNode(followee);
			Node* node2 = manager->getNode(follower);
			Relation* rel = manager->createRelation(i,"follow");
			node2->addRelationTo(*node1,*rel);
			cout<<"Follow complete. Form "<<follower<<" to "<<followee<<endl;

			manager->commit(node1);
			manager->commit(node2);
			manager->commit(rel);
			i++;
			destroy(node1);
			destroy(node2);
			destroy(rel);

		}else{
			Node* node1 = manager->getNode(follower);
			Node* node2 = manager->getNode(followee);
			Relation* rel = manager->createRelation(i,"follow");
			node1->addRelationTo(*node2,*rel);
			cout<<"Follow complete. Form "<<follower<<" to "<<followee<<endl;

			manager->commit(node1);
			manager->commit(node2);
			manager->commit(rel);
			i++;
			destroy(node1);
			destroy(node2);
			destroy(rel);
		}
	}

	//test time
	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout<<"Finish to construct a graph, all data from twitter. Time cost: "<< diff << "seconds" <<endl;

	//close file
	fclose(t_file);
}
static void benchmarkTwitterUnfollow(long int follower,long int followee){

	//get id of relation between follower and followee. Then manager remove this relation.
	Manager* manager = Manager::getInstance();
	clock_t start = clock();
	Node* node1 = manager->getNode(follower);
	set<string> in_rel = node1->getRelations("follow",OUT);

	set::iterator it;
	long int &result;
	for(it == in_rel.begin();it != in_rel.end();it++){
		if(manager->lookup(in_rel[it],followee,result)){
			//match followee and follower, delete relation.
			manager->removeRelation(result);
			break;
		}
	}
	destroy(node1);

	//time benchmark
	double diff = (clock() - start) / (double) CLOCKS_PER_SEC;
	cout<<"Finish un-follow. Time cost: "<<diff<<"seconds"<<endl;
}
static void benchmarkTwitterDeleteNode(long int id){

	//delete one node in twitter graph.
	Manager* manager = Manager::getInstance();
}

static void testDone()
{
	Manager *manager = Manager::getInstance();
	manager->close();
	cout << "Test Done" << endl;
}
