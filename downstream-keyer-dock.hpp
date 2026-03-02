#pragma once
#include <QDockWidget>
#include <qmenu.h>
//#include <QTabWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QFrame>
#include <obs-frontend-api.h>
#include "downstream-keyer.hpp"
#include "obs-websocket-api.h"

enum class DownstreamKeyerStyle { Tabs, List };

class DownstreamKeyerContainer : public QWidget {
    Q_OBJECT
public:
    DownstreamKeyerContainer(DownstreamKeyerStyle style = DownstreamKeyerStyle::Tabs,
                             Qt::Orientation listOrient = Qt::Vertical,
                             QWidget *parent = nullptr);

    int            count() const;
    QWidget       *widget(int idx) const;
    QString        name(int idx) const;
    void           setName(int idx, const QString &text);
    int            currentIndex() const;
    QWidget       *currentWidget() const;
    void           setCurrentIndex(int idx);
    int            addPage(QWidget *page, const QString &name);
    void           removePage(int idx);
    void           clear();

    void           setMovable(bool movable);
    void           setCornerWidget(QWidget *widget);

    void           setStyle(DownstreamKeyerStyle style,
                            Qt::Orientation listOrient = Qt::Vertical);

    DownstreamKeyerStyle getStyle() const { return m_style; }

signals:
    void currentChanged(int idx);
    void pageMoved(int from, int to);

private:
    DownstreamKeyerStyle   m_style;
    Qt::Orientation        m_listOrientation;

    QStackedWidget        *m_stack{nullptr};
    QListWidget           *m_list{nullptr};

	QHBoxLayout			  *m_h_layout{nullptr};
	QVBoxLayout			  *m_v_layout{nullptr};
    QTabWidget            *m_tabs{nullptr};


    QStringList           *m_names{nullptr};      // keep text for both selectors
	QWidget				  *m_config_widget{nullptr};
};

class DownstreamKeyerDock : public QFrame {
	Q_OBJECT
private:
	DownstreamKeyerContainer *keyers;
	DownstreamKeyerStyle selectorStyle = DownstreamKeyerStyle::Tabs;
	Qt::Orientation selectorOrientation = Qt::Vertical;
	int outputChannel;
	bool loaded = false;
	bool closing = false;
	obs_view_t *view = nullptr;
	obs_weak_canvas_t *canvas = nullptr;
	std::string viewName;
	get_transitions_callback_t get_transitions = nullptr;
	void *get_transitions_data = nullptr;

	void Save(obs_data_t *data);
	void Load(obs_data_t *data);
	QString GetScene(QString dskName);
	bool SwitchDSK(QString dskName, QString sceneName);
	bool AddScene(QString dskName, QString sceneName, int insertBeforeRow);
	bool RemoveScene(QString dskName, QString sceneName);
	bool SetTie(QString dskName, bool tie);
	bool SetTransition(const QString &chars, const char *transition, int duration, transitionType tt);
	bool AddExcludeScene(QString dskName, const char *sceneName);
	bool RemoveExcludeScene(QString dskName, const char *sceneName);

	void ClearKeyers();
	void AddDefaultKeyer();
	void ConfigClicked();
	void AddTransitionMenu(QMenu *tm, enum transitionType transition_type);
	void AddExcludeSceneMenu(QMenu *tm);
private slots:
	void SceneChanged();
	void Add(QString name = "");
	void Rename();
	void Remove(int index = -1);

public:
	DownstreamKeyerDock(QWidget *parent = nullptr, int outputChannel = 7, obs_view_t *view = nullptr,
			    obs_canvas_t *canvas = nullptr, const char *view_name = nullptr);
	~DownstreamKeyerDock();

	void setDownstreamKeyerStyle(DownstreamKeyerStyle style,
                                           Qt::Orientation orient);

	void SetTransitions(get_transitions_callback_t get_transitions = nullptr, void *get_transitions_data = nullptr);

	inline obs_view_t *GetView() { return view; }
	inline obs_canvas_t *GetCanvas() { return obs_weak_canvas_get_canvas(canvas); }

	static void frontend_event(enum obs_frontend_event event, void *data);
	static void frontend_save_load(obs_data_t *save_data, bool saving, void *data);

	static void get_downstream_keyers(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void get_downstream_keyer(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void add_downstream_keyer(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void remove_downstream_keyer(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void get_scene(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void change_scene(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void add_scene(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void remove_scene(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void set_tie(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void set_transition(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void add_exclude_scene(obs_data_t *request_data, obs_data_t *response_data, void *param);
	static void remove_exclude_scene(obs_data_t *request_data, obs_data_t *response_data, void *param);
};
