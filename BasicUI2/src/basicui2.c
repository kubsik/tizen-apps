#include "basicui2.h"

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *nf;
	Evas_Object *box;
	Evas_Object *button_pop;
	Evas_Object *popup;

} appdata_s;

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

void
cancel_cb(appdata_s *ad, Evas_Object *obj, void *event_info)
{
	dlog_print(DLOG_DEBUG, LOG_TAG, "after reading ad pointer");
	evas_object_hide(ad->popup);
	dlog_print(DLOG_DEBUG, LOG_TAG, "after hideing popup");
}

void
show_cb(appdata_s *ad, Evas_Object *obj, void *event_info)
{
	dlog_print(DLOG_DEBUG, LOG_TAG, "after reading ad pointer");
	evas_object_show(ad->popup);
	dlog_print(DLOG_DEBUG, LOG_TAG, "after showing popup");
}

static void
create_base_gui(appdata_s *ad)
{
	/* Window */
	/* Create and initialize elm_win.
	   elm_win is mandatory to manipulate window. */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

	/* Conformant */
	/* Create and initialize elm_conformant.
	   elm_conformant is mandatory for base gui to have proper size
	   when indicator or virtual keypad is visible. */
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	/* Box */
	/* Create an actual view of the base gui.
	   Modify this part to change the view. */
	ad->box = elm_box_add(ad->conform);
//	elm_box_horizontal_set(ad->box, EINA_TRUE);
//	evas_object_size_hint_weight_set(ad->box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//	evas_object_size_hint_align_set(ad->box, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_content_set(ad->conform, ad->box);
    evas_object_show(ad->box);
    // dont have here navi frame
    //	elm_naviframe_item_push(ad->conform, "Box", NULL, NULL, ad->box, NULL);

	/* Create a button_pop */
	ad->button_pop = elm_button_add(ad->box);
	elm_object_style_set(ad->button_pop, "circle");
	elm_object_text_set(ad->button_pop, "show");
//	evas_object_size_hint_weight_set(ad->button_pop, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(ad->button_pop);
	evas_object_smart_callback_add(ad->button_pop, "clicked", show_cb, ad);
	elm_box_pack_end(ad->box, ad->button_pop);


	/* Create a popup */
		ad->popup = elm_popup_add(ad->box);
		elm_popup_align_set(ad->popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
		evas_object_size_hint_weight_set(ad->popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		elm_object_part_text_set(ad->popup, "title,text", "Popup Title");
		elm_object_text_set(ad->popup, "Very important shit");
//		evas_object_show(ad->popup);


		Evas_Object *button_OK;
		Evas_Object *button_Cancel;

		button_OK = elm_button_add(ad->popup);
		elm_object_style_set(button_OK, "popup");
		elm_object_text_set(button_OK, "OK");
		evas_object_show(button_OK);

		button_Cancel = elm_button_add(ad->popup);
		elm_object_text_set(button_Cancel, "Cancel");

		elm_object_part_content_set(ad->popup, "button1", button_OK);
		elm_object_part_content_set(ad->popup, "button2", button_Cancel);
		evas_object_smart_callback_add(button_OK, "clicked", cancel_cb, ad);
		evas_object_smart_callback_add(button_Cancel, "clicked", cancel_cb, ad);
		evas_object_show(button_Cancel);
		evas_object_show(button_OK);

	/* Show window after base gui is set up */
	evas_object_show(ad->win);
}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	appdata_s *ad = data;

	create_base_gui(ad);

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/

	int ret;
	char *language;

	ret = app_event_get_language(event_info, &language);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_event_get_language() failed. Err = %d.", ret);
		return;
	}

	if (language != NULL) {
		elm_language_set(language);
		free(language);
	}
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

int
main(int argc, char *argv[])
{
	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;
}
