// Goodix Tls driver for libfprint

// Copyright (C) 2021 Alexander Meiler <alex.meiler@protonmail.com>
// Copyright (C) 2021 Matthieu CHARETTE <matthieu.charette@gmail.com>

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#pragma once

// 1 seconds USB timeout
#define GOODIX_TIMEOUT (1000)

G_DECLARE_DERIVABLE_TYPE(FpiDeviceGoodixTls, fpi_device_goodixtls, FPI,
                         DEVICE_GOODIXTLS, FpImageDevice)

#define FPI_TYPE_DEVICE_GOODIXTLS (fpi_device_goodixtls_get_type())

struct _FpiDeviceGoodixTlsClass {
  FpImageDeviceClass parent;

  gint interface;
  guint8 ep_in;
  guint8 ep_out;
};

typedef struct __attribute__((__packed__)) _GoodixCallbackInfo {
  GCallback callback;
  gpointer user_data;
} GoodixCallbackInfo;

typedef void (*GoodixCmdCallback)(FpDevice *dev, guint8 *data, guint16 length,
                                  gpointer user_data, GError *error);

typedef void (*GoodixFirmwareVersionCallback)(FpDevice *dev, gchar *firmware,
                                              gpointer user_data,
                                              GError *error);

typedef void (*GoodixPresetPskReadCallback)(FpDevice *dev, gboolean success,
                                            guint32 flags, guint8 *psk,
                                            guint16 length, gpointer user_data,
                                            GError *error);

typedef void (*GoodixSuccessCallback)(FpDevice *dev, gboolean success,
                                      gpointer user_data, GError *error);

typedef void (*GoodixResetCallback)(FpDevice *dev, gboolean success,
                                    guint16 number, gpointer user_data,
                                    GError *error);

typedef void (*GoodixNoneCallback)(FpDevice *dev, gpointer user_data,
                                   GError *error);

typedef void (*GoodixDefaultCallback)(FpDevice *dev, guint8 *data,
                                      guint16 length, gpointer user_data,
                                      GError *error);
typedef GoodixDefaultCallback GoodixTlsCallback;

typedef void (*GoodixImageCallback)(FpDevice* dev, guint8* data, guint16 length,
                                    gpointer user_data, GError* error);

gchar *data_to_str(guint8 *data, guint32 length);

// ---- GOODIX RECEIVE SECTION START ----

void goodix_receive_done(FpDevice *dev, guint8 *data, guint16 length,
                         GError *error);

void goodix_receive_success(FpDevice *dev, guint8 *data, guint16 length,
                            gpointer user_data, GError *error);

void goodix_receive_reset(FpDevice *dev, guint8 *data, guint16 length,
                          gpointer user_data, GError *error);

void goodix_receive_none(FpDevice *dev, guint8 *data, guint16 length,
                         gpointer user_data, GError *error);

void goodix_receive_default(FpDevice *dev, guint8 *data, guint16 length,
                            gpointer user_data, GError *error);

void goodix_receive_preset_psk_read(FpDevice *dev, guint8 *data, guint16 length,
                                    gpointer user_data, GError *error);

void goodix_receive_preset_psk_write(FpDevice *dev, guint8 *data,
                                     guint16 length, gpointer user_data,
                                     GError *error);

void goodix_receive_ack(FpDevice *dev, guint8 *data, guint16 length,
                        gpointer user_data, GError *error);

void goodix_receive_firmware_version(FpDevice *dev, guint8 *data,
                                     guint16 length, gpointer user_data,
                                     GError *error);

void goodix_receive_protocol(FpDevice *dev, guint8 *data, guint32 length);

void goodix_receive_pack(FpDevice *dev, guint8 *data, guint32 length);

void goodix_receive_data_cb(FpiUsbTransfer *transfer, FpDevice *dev,
                            gpointer user_data, GError *error);

void goodix_receive_timeout_cb(FpDevice *dev, gpointer user_data);

void goodix_receive_data(FpDevice *dev);

void goodix_start_read_loop(FpDevice* dev);
// ---- GOODIX RECEIVE SECTION END ----

// -----------------------------------------------------------------------------

// ---- GOODIX SEND SECTION START ----

gboolean goodix_send_data(FpDevice *dev, guint8 *data, guint32 length,
                          GDestroyNotify free_func, GError **error);

gboolean goodix_send_pack(FpDevice *dev, guint8 flags, guint8 *payload,
                          guint16 length, GDestroyNotify free_func,
                          GError **error);

void goodix_send_protocol(FpDevice *dev, guint8 cmd, guint8 *payload,
                          guint16 length, GDestroyNotify free_func,
                          gboolean calc_checksum, guint timeout_ms,
                          gboolean reply, GoodixCmdCallback callback,
                          gpointer user_data);

void goodix_send_nop(FpDevice *dev, GoodixNoneCallback callback,
                     gpointer user_data);

void goodix_send_mcu_get_image(FpDevice *dev, guint8* payload, guint16 length, GoodixImageCallback callback,
                               gpointer user_data);

void goodix_send_mcu_switch_to_fdt_down(FpDevice *dev, guint8 *mode,
                                        guint16 length, gboolean reply,
                                        GDestroyNotify free_func,
                                        GoodixDefaultCallback callback,
                                        gpointer user_data);

void goodix_send_mcu_switch_to_fdt_up(FpDevice *dev, guint8 *mode,
                                      guint16 length, gboolean reply, GDestroyNotify free_func,
                                      GoodixDefaultCallback callback,
                                      gpointer user_data);

void goodix_send_mcu_switch_to_fdt_mode(FpDevice *dev, guint8 *mode,
                                        guint16 length, gboolean reply,
                                        GDestroyNotify free_func,
                                        GoodixDefaultCallback callback,
                                        gpointer user_data);

void goodix_send_nav_0(FpDevice *dev, GoodixDefaultCallback callback,
                       gpointer user_data);

void goodix_send_mcu_switch_to_idle_mode(FpDevice *dev, guint8 sleep_time,
                                         GoodixNoneCallback callback,
                                         gpointer user_data);

void goodix_send_write_sensor_register(FpDevice *dev, guint16 address,
                                       guint16 value,
                                       GoodixNoneCallback callback,
                                       gpointer user_data);

void goodix_send_read_sensor_register(FpDevice *dev, guint16 address,
                                      guint8 length,
                                      GoodixDefaultCallback callback,
                                      gpointer user_data);

void goodix_send_upload_config_mcu(FpDevice *dev, guint8 *config,
                                   guint16 length, GDestroyNotify free_func,
                                   GoodixSuccessCallback callback,
                                   gpointer user_data);

void goodix_send_set_powerdown_scan_frequency(FpDevice *dev,
                                              guint16 powerdown_scan_frequency,
                                              GoodixSuccessCallback callback,
                                              gpointer user_data);

void goodix_send_enable_chip(FpDevice *dev, gboolean enable,
                             GoodixNoneCallback callback, gpointer user_data);

void goodix_send_reset(FpDevice *dev, gboolean reset_sensor, guint8 sleep_time,
                       GoodixResetCallback callback, gpointer user_data);

void goodix_send_firmware_version(FpDevice *dev,
                                  GoodixFirmwareVersionCallback callback,
                                  gpointer user_data);

void goodix_send_query_mcu_state(FpDevice *dev, GoodixDefaultCallback callback,
                                 gpointer user_data);

void goodix_send_request_tls_connection(FpDevice* dev,
                                        GoodixDefaultCallback callback,
                                        gpointer user_data);

void goodix_send_tls_successfully_established(FpDevice *dev,
                                              GoodixNoneCallback callback,
                                              gpointer user_data);

void goodix_send_preset_psk_write(FpDevice *dev, guint32 flags, guint8 *psk,
                                  guint16 length, GDestroyNotify free_func,
                                  GoodixSuccessCallback callback,
                                  gpointer user_data);

void goodix_send_preset_psk_read(FpDevice *dev, guint32 flags, guint16 length,
                                 GoodixPresetPskReadCallback callback,
                                 gpointer user_data);

void goodix_send_read_otp(FpDevice* dev, GoodixDefaultCallback callback,
                          gpointer user_data);

// ---- GOODIX SEND SECTION END ----

// -----------------------------------------------------------------------------

// ---- DEV SECTION START ----

gboolean goodix_dev_init(FpDevice *dev, GError **error);

gboolean goodix_dev_deinit(FpDevice *dev, GError **error);

void goodix_reset_state(FpDevice* dev);

// ---- DEV SECTION END ----

// -----------------------------------------------------------------------------

// ---- TLS SECTION START ----

void goodix_read_tls(FpDevice* dev, GoodixTlsCallback callback,
                     gpointer user_data);

void goodix_tls_run_state(FpiSsm* ssm, FpDevice* dev);

void goodix_tls_complete(FpiSsm *ssm, FpDevice *dev, GError *error);

void goodix_tls(FpDevice* dev, GoodixNoneCallback callback, gpointer user_data);

gboolean goodix_shutdown_tls(FpDevice* dev, GError** error);

void goodix_tls_read_image(FpDevice* dev, guint8* payload, guint16 length, GoodixImageCallback callback,
                           gpointer user_data);

void goodix_tls_decrypt_image(FpDevice* dev, guint8** data, guint16* len);

// ---- TLS SECTION END ----
