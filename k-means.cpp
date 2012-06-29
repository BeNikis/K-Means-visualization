

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdlib>



	class Clusters {
		
		std::vector<sf::CircleShape> clusters;
		int cnum;
		bool clusters_stable;
		public:
			
			
			Clusters(int cluster_num) :cnum(cluster_num) , clusters_stable(false){
				
				for (int c=0;c<cnum;c++) {
					clusters.push_back(sf::CircleShape(10));
					clusters[c].SetPosition(std::rand()%800,std::rand()%600);
					clusters[c].SetFillColor(sf::Color(std::rand()%255,std::rand()%255,std::rand()%255));
				};
				
			};
			
			
			void draw(sf::RenderWindow& w) {
				for (int i=0;i<cnum;i++)
					w.Draw(clusters[i]);
			};
			
			
			void adjust_clusters(std::vector<sf::CircleShape>& points) {
				std::vector< std::vector<sf::CircleShape*>* > elems;
				for (int i=0;i<cnum;i++) {
				elems.push_back(new std::vector<sf::CircleShape*>);
				};
				
				if (!clusters_stable) {
					
					
					//Nearest cluster for a point
					for (unsigned int p=0;p<points.size();p++) {
						float distance=999999;
						int nearest_cluster=0;
						
						//std::cout << points[p].GetPosition().x << " " << points[p].GetPosition().y << " ";
						for (int c=0;c<cnum;c++) {
							
							float c_distance=sqrt(pow(clusters[c].GetPosition().x - points[p].GetPosition().x,2)+pow(clusters[c].GetPosition().y-points[p].GetPosition().y,2));
							//std::cout << c_distance << std::endl;
							if (c_distance<distance) {
								distance=c_distance;
								nearest_cluster=c;
							};
						};
					elems[nearest_cluster]->push_back(&points[p]);
					
				};
				
				bool stable_iteration=true;
				//color points based on the color of the assigned cluster and move the cluster to its new position
				for (int c=0;c<cnum;c++) {
					
					sf::Vector2f newpos(0.f,0.f);
					
					for(unsigned int p=0;p<elems[c]->size();p++) {
						newpos+=(*elems[c])[p]->GetPosition();
						
						(*elems[c])[p]->SetFillColor(clusters[c].GetFillColor()+sf::Color(20,20,20));
					};
					
					newpos/=(float)elems[c]->size();
					
					if (!(stable_iteration =stable_iteration && (clusters[c].GetPosition()==newpos)))	
						clusters[c].SetPosition(newpos);
				};
				clusters_stable=stable_iteration;
			};
			//while(elems.size()!=0) elems.pop_back();
			for (int i=0;i<elems.size();i++) std::cout << elems[i]->size() << " ";
			std::cout  << "\n";
		};
					
					
		void reset(int cluster_num) {
			clusters_stable=false;
			
			int onum=clusters.size();
			if (cluster_num<=cnum) {
				for (int i=0;i<cnum-onum;i++) clusters.pop_back();
			
			cnum=cluster_num;
			
			
			}
			else {
				while (cnum!=cluster_num) {
					clusters.push_back(sf::CircleShape(10));
					cnum++;
				};	
			};
			
			for (int c=0;c<cnum;c++) {
					clusters[c].SetPosition(std::rand()%800,std::rand()%600);
					clusters[c].SetFillColor(sf::Color(std::rand()%255,std::rand()%255,std::rand()%255));
				};
				
			
		};
		
		
		const std::vector<sf::CircleShape>& getClusters() {
			return clusters;
		};
		
		
		bool clustersStable() { return clusters_stable;};		
		};
				

int pressedDigit(sf::Event e) {
	sf::Keyboard::Key num=e.Key.Code;
	if ((num >= sf::Keyboard::Num0) && (num <= sf::Keyboard::Num9)) 
		return num-sf::Keyboard::Num0;
	else if	((num >= sf::Keyboard::Numpad0) &&	(num <= sf::Keyboard::Numpad9))
		return num-sf::Keyboard::Numpad0;
	else
		return -1;
	};


int main(int argc, char** argv)
{
	
	sf::RenderWindow win(sf::VideoMode(800,600,32),"K-Cluster algorithm visualization");
	win.SetFramerateLimit(5);
	sf::Event mainloop;
	std::vector<sf::CircleShape> points;
	std::srand(time(NULL));
	
	
	for (int i=0;i<100;i++) {
		points.push_back(sf::CircleShape(5));
		points[i].SetPosition(std::rand()%800,std::rand()%600);
		points[i].SetFillColor(sf::Color::White);
		
	};
	
	int numc=5;	
	Clusters clu(numc);
	
	while (win.IsOpen()) {
		win.Clear();
		while (win.PollEvent(mainloop)) {
			if (mainloop.Type==sf::Event::Closed) win.Close();
			
			if (mainloop.Type==sf::Event::KeyPressed) {
				int pressed = pressedDigit(mainloop) ;
				if (pressed != -1 || pressed != 0 || pressed != 1) numc=pressed;
			};
		};
		
		if (clu.clustersStable()) {
			std::cout << numc << "\n";
			clu.reset(numc);
		} 
		
		clu.adjust_clusters(points);
		
		for (unsigned int i=0;i<points.size();i++) {
			//points[i].Move((std::rand()%7)-3,(std::rand()%7)-3);
			win.Draw(points[i]);
		};
		
		
		
		clu.draw(win);
		win.Display();
		
		
		
		
		
			

		
		
	};
		
	return 0;
}
