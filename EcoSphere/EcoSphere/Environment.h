#include "ecosystem_gdiplus.h"
#include "EcoSystemObject.h"
#include "Vector2D.h"

/*
 *环境
*/
#ifndef DS_ECOSYSTEM_ENVIRONMENT
#define DS_ECOSYSTEM_ENVIRONMENT

class EcoSystem;
class Entity;
class Producer;
class FoodWeb;

class Environment : public DynamicEcoSystemObject
{
public:
	Environment(EcoSystem *eco_system);
	virtual ~Environment();
	virtual void    render_environment_background(Gdiplus::Graphics *g) const;//渲染环境的背景效果
	virtual void    render_environment_effects(Gdiplus::Graphics *g) const;//渲染环境的其他效果
	virtual void	on_tick();//由EcoSystem的on_tick函数 调用
	virtual void	spawn_producer();//在生态系统中生成生产者（比如草）

	//环境因素（亮度、温度、相对湿度、降雨量）的get和set函数
	double      get_brightness() const;
	void        set_brightness(double brightness);
	double      get_temperature() const;
	void        set_temperature(double temperature);
	double      get_relative_humidity() const;
	void        set_relative_humidity(double relative_humidity);
	double      get_precipitation() const;
	void        set_precipitation(double precipitation);

	//环境中的生产者。为了简化模型，只有一种生产者。
	Producer *get_default_producer() const;
	void		set_default_producer(Producer *producer);

protected:
private:
	double brightness, temperature, relative_humidity, precipitation;// brightness(0 - 100), tempreature(degree celsius), relative Humidity(0.0 ~ 1.0), precipitation(millimeter)
	Producer *default_producer;
};

#endif // DS_ECOSYSTEM_ENVIRONMENT
