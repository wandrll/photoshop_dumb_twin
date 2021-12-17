#include "level_widget.hpp"


Spline_widget::Spline_widget (const int x, const int y, const int width, const int height) : 
    Window_widget(x, y, width, height){

    Spline_field* field = new Spline_field(30, 30, width - 60, height - 60, 10);
    this->widgets.push_back(field);


}

Spline_field::Spline_field (const int x, const int y, const int width, const int height, const int cnt) : 
    Widget_manager(x, y, width, height),
    texture(width, height){
    
    Rectangle_widget* bakground = new Rectangle_widget(0, 0, width, height, Color(40, 40, 40, 200));
    this->widgets.push_back(bakground);

    texture.clear();


    for (int i = 0; i < cnt; i++){
        Controller<Move_slider, Spline_field>* control = new Controller<Move_slider, Spline_field>(this);
        Slider_for_field* curr_slider = new Slider_for_field(i * ((double)width)/((double)cnt), height/2 - 10, 20, 20, 0, 0, width, height, &sliders);
        curr_slider->add_controller(control);

        sliders.push_back(curr_slider);
        this->widgets.push_back(curr_slider);
    }
    sort_sliders();
    update_points();
    update_texture();



}

void Spline_field::sort_sliders(){
    std::sort(sliders.begin(), sliders.end(), 
            [](const Slider* a, const Slider* b){
                Vector first = a->get_value();
                Vector second = b->get_value();
                if(first.x < second.x){
                    return true;
                }

                if(first.x == second.x && first.y < second.y){
                    return true;
                }

                return false;
            });

}

void Spline_field::update_points(){
    points.clear();

    points.push_back(Vector(-1, height/2));
    points.push_back(Vector(0, height/2));

    // Vector size = get_size();
    
    for (int i = 0; i < sliders.size(); i++){
        Vector curr = sliders[i]->get_value();

        curr.x *= width;
        curr.y *= height;

        points.push_back(curr);
    }

    points.push_back(Vector(width, height/2));
    points.push_back(Vector(width + 1, height/2));


}


double GetT(const double t, const double alpha, const Vector& p0, const Vector& p1 ){
    double a = pow((p1.x-p0.x), 2.0f) + pow((p1.y-p0.y), 2.0f);
	double b = pow(a, alpha * 0.5f);
	   
	return (b + t);
}

Vector CatmullRom(const Vector& p0, const Vector& p1, const Vector& p2, const Vector& p3, double t /* between 0 and 1 */, double alpha = 0.5 /* between 0 and 1 */ ){


    double t0 = 0.0;
    double t1 = GetT( t0, alpha, p0, p1 );
    double t2 = GetT( t1, alpha, p1, p2 );
    double t3 = GetT( t2, alpha, p2, p3 );

    t = t1 + (t2 - t1) * t;


    Vector A1 = p0 * (( t1-t )/( t1-t0 )) + p1 * (( t-t0 )/( t1-t0 ));
    Vector A2 = p1 * (( t2-t )/( t2-t1 )) + p2 * (( t-t1 )/( t2-t1 ));
    Vector A3 = p2 * (( t3-t )/( t3-t2 )) + p3 * (( t-t2 )/( t3-t2 ));
    
    Vector B1 = A1 * (( t2-t )/( t2-t0 )) + A2 * (( t-t0 )/( t2-t0 ));
    Vector B2 = A2 * (( t3-t )/( t3-t1 )) + A3 * (( t-t1 )/( t3-t1 ));
    
    Vector C  = B1 * (( t2-t )/( t2-t1 )) + B2 * (( t-t1 )/( t2-t1 ));
    return C;
}


void Spline_field::update_texture(){
    texture.clear();

    Circle circle;
    circle.set_size(1);
    circle.set_color({255, 0, 0, 64});
    // printf("\n----------------------\n");
    Rectangle rect;
    rect.set_color({255, 0, 0, 64});
        
    for (int i = 0; i < points.size() - 3; i++){
        Vector first  = points[i];
        Vector second = points[i + 1];
        Vector third  = points[i + 2];
        Vector fourth = points[i + 3];
        
        
        for(int j = second.x; j < third.x; j++){
            Vector coords = CatmullRom(first, second, third, fourth, (((double)j) - second.x) /(third.x - second.x));
            circle.set_position({j, coords.y});
            circle.draw(texture, 0, 0);

            rect.set_position({j, coords.y});
            rect.set_size(1, height - coords.y);
            rect.draw(texture, 0, 0);

        }
        // draw_between_points(circle, first, second, texture);
    }


}


bool Spline_field::on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event){
    bool res = Widget_manager::on_right_mouse_press(x, y, event);
    if(res){
        return true;
    }

    if(is_in_widget(x, y, event.click)){
        Controller<Move_slider, Spline_field>* control = new Controller<Move_slider, Spline_field>(this);
        Slider_for_field* curr_slider = new Slider_for_field(event.click.x - x -this->x - 10, event.click.y - y - this->y - 10, 20, 20, 0, 0, width, height, &sliders);
        curr_slider->add_controller(control);

        sliders.push_back(curr_slider);
        this->widgets.push_back(curr_slider);

        sort_sliders();
        update_points();
        update_texture();
        
        return true;
    }
    return false;


}


void Spline_field::draw (const int x, const int y, Texture& window){
    Widget_manager::draw(x, y, window);


    Sprite sprite;
    sprite.set_texture(this->texture);
    
    sprite.draw(window, this->x + x, this->y + y);
  

}


Slider_for_field::Slider_for_field (const int x, const int y, const int width, const int height, const int left_x_limit, const int top_y_limit, const int right_x_limit, const int bottom_y_limit, std::vector<Slider_for_field*>* ptr) :
    Slider(x, y, width, height, left_x_limit, top_y_limit, right_x_limit, bottom_y_limit),
    vector_ptr(ptr)
{}

Slider_for_field::~Slider_for_field (){

}

bool Slider_for_field::on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event){
    if(is_in_widget(x, y, event.click)){   
        mark_for_delete = true;


        for (int i = 0; i < vector_ptr->size(); i++){
            if((*vector_ptr)[i] == this){
                std::swap((*vector_ptr)[i], (*vector_ptr)[vector_ptr->size() - 1]);
                vector_ptr->pop_back();
                break;
            }
        }

        return true;
    }
    return false;
}
