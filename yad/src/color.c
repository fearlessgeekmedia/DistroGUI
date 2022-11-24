/*
 * This file is part of YAD.
 *
 * YAD is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * YAD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with YAD. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2008-2021, Victor Ananjevsky <ananasik@gmail.com>
 */

#include <errno.h>

#include "yad.h"
#include "cpicker.h"

static GtkWidget *color;

static GtkTreeModel *
create_palette ()
{
  FILE *fp;
  gchar *filename;
  gchar buf[1024], *name;
  GtkListStore *store;
  GdkPixbuf *pb;
  GtkTreeIter iter;

  gchar *cxpm[] = {
    "16 14 1 1",
    ". c #FFFFFF",
    "................",
    "................",
    "................",
    "................",
    "................",
    "................",
    "................",
    "................",
    "................",
    "................",
    "................",
    "................",
    "................",
    "................"
  };
  gchar cl[] = ". c #FFFFFF";

  if (options.color_data.palette)
    filename = options.color_data.palette;
  else
    filename = RGB_FILE;

  if ((fp = fopen (filename, "r")) == NULL)
    {
      g_printerr (_("Can't open file %s: %s\n"), filename, strerror (errno));
      return NULL;
    }

  store = gtk_list_store_new (3, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING);

  while ((name = fgets (buf, sizeof (buf), fp)) != NULL)
    {
      gint r, g, b;
      gchar spec[8];

      if (buf[0] == '!')
        continue;

      sscanf (buf, "%d %d %d", &r, &g, &b);

      g_sprintf (spec, "#%.2X%.2X%.2X", r, g, b);
      g_sprintf (cl, ". c #%.2X%.2X%.2X", r, g, b);
      cxpm[1] = cl;
      pb = gdk_pixbuf_new_from_xpm_data ((const gchar **) cxpm);

      while (g_ascii_isdigit (*name) || g_ascii_isspace (*name))
        name++;

      gtk_list_store_append (store, &iter);
      gtk_list_store_set (store, &iter, 0, pb, 1, spec, 2, g_strstrip (name), -1);
    }

  fclose (fp);

  return GTK_TREE_MODEL (store);
}

static void
set_color (gchar *clr)
{
  GdkRGBA c;
  if (gdk_rgba_parse (&c, clr))
    gtk_color_chooser_set_rgba (GTK_COLOR_CHOOSER (color), &c);
}

static void
picker_clicked (GtkWidget *w, gpointer data)
{
  yad_get_screen_color (color);
}

static void
palette_changed (GtkTreeSelection * sel, gpointer data)
{
  GtkTreeModel *model;
  GtkTreeIter iter;

  if (gtk_tree_selection_get_selected (sel, &model, &iter))
    {
      gchar *clr;
      gtk_tree_model_get (model, &iter, 1, &clr, -1);
      set_color (clr);
    }
}

GtkWidget *
color_create_widget (GtkWidget * dlg)
{
  GtkWidget *w;

  w = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
  color = gtk_color_chooser_widget_new ();
  gtk_widget_set_name (color, "yad-color-widget");
  g_object_set (G_OBJECT (color), "show-editor", !options.color_data.gtk_palette, NULL);

  if (options.color_data.init_color)
    set_color (options.color_data.init_color);
  gtk_box_pack_start (GTK_BOX (w), color, FALSE, FALSE, 2);

  if (options.color_data.color_picker)
    {
      GtkWidget *b, *i;

      b = gtk_button_new_with_label (_("Pick up screen color"));

      i = gtk_image_new_from_icon_name ("gtk-color-picker", GTK_ICON_SIZE_BUTTON);
      gtk_button_set_image (GTK_BUTTON (b), i);
      g_object_set (G_OBJECT (b), "always-show-image", TRUE, NULL);

      g_signal_connect (G_OBJECT (b), "clicked", G_CALLBACK (picker_clicked), NULL);

      gtk_box_pack_start (GTK_BOX (w), b, FALSE, FALSE, 2);
    }

  if (options.color_data.use_palette)
    {
      GtkTreeModel *model;

      if ((model = create_palette ()) != NULL)
        {
          GtkWidget *exp, *sw, *list;
          GtkCellRenderer *r;
          GtkTreeViewColumn *col;
          GtkTreeSelection *sel;

          /* create expander */
          exp = gtk_expander_new (_("Palette"));
          gtk_expander_set_expanded (GTK_EXPANDER (exp), options.color_data.expand_palette);
          gtk_container_set_border_width (GTK_CONTAINER (exp), 5);
          gtk_box_pack_start (GTK_BOX (w), exp, TRUE, TRUE, 2);

          /* create color list */
          sw = gtk_scrolled_window_new (NULL, NULL);
          gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw), GTK_SHADOW_ETCHED_IN);
          gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw), options.data.hscroll_policy, options.data.vscroll_policy);
          gtk_widget_set_size_request (sw, -1, 75);
          gtk_container_add (GTK_CONTAINER (exp), sw);
          gtk_widget_set_vexpand (sw, TRUE);
          list = gtk_tree_view_new_with_model (model);
          gtk_widget_set_name (list, "yad-color-palette");
          g_object_unref (model);
          gtk_container_add (GTK_CONTAINER (sw), list);

          /* add first columns */
          col = gtk_tree_view_column_new ();
          gtk_tree_view_column_set_title (col, _("Color"));

          /* pixbuf */
          r = gtk_cell_renderer_pixbuf_new ();
          gtk_tree_view_column_pack_start (col, r, FALSE);
          gtk_tree_view_column_set_attributes (col, r, "pixbuf", 0, NULL);

          /* color value */
          r = gtk_cell_renderer_text_new ();
          gtk_tree_view_column_pack_start (col, r, TRUE);
          gtk_tree_view_column_set_attributes (col, r, "text", 1, NULL);

          gtk_tree_view_column_set_sort_column_id (col, 1);
          gtk_tree_view_append_column (GTK_TREE_VIEW (list), col);

          /* add second column */
          r = gtk_cell_renderer_text_new ();
          col = gtk_tree_view_column_new_with_attributes (_("Name"), r, "text", 2, NULL);
          gtk_tree_view_column_set_sort_column_id (col, 2);
          gtk_tree_view_append_column (GTK_TREE_VIEW (list), col);

          /* enable searching on Name column */
          gtk_tree_view_set_search_column (GTK_TREE_VIEW (list), 2);

          /* setup the selection handler */
          sel = gtk_tree_view_get_selection (GTK_TREE_VIEW (list));
          gtk_tree_selection_set_mode (sel, GTK_SELECTION_SINGLE);
          g_signal_connect (G_OBJECT (sel), "changed", G_CALLBACK (palette_changed), NULL);

          gtk_widget_show_all (exp);
        }
    }

  return w;
}

void
color_print_result (void)
{
  GdkRGBA c;
  gchar *cs;

  gtk_color_chooser_get_rgba (GTK_COLOR_CHOOSER (color), &c);
  cs = get_color (&c);

  if (cs)
    g_printf ("%s\n", cs);
}
