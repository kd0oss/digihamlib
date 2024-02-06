#ifndef DIGIHAMLIB_H
#define DIGIHAMLIB_H

#include "digihamlib_global.h"
#include <QObject>
#include <QSettings>
#include "mode.h"

class DIGIHAMLIB_EXPORT Digihamlib : public QObject
{
    Q_OBJECT
public:
    explicit Digihamlib(QObject *parent = nullptr);
    ~Digihamlib();

    Mode *m_mode;

    QString m_vocoder;
    QString m_modem;
    QStringList m_vocoders;
    QStringList m_modems;
    QStringList m_hostsmodel;
    QMap<QString, QString> m_hostmap;
    QStringList m_customhosts;
    QQueue<int16_t> m_rxaudioq;
    SerialAMBE *m_ambedev;

signals:
    void tx_pressed();
    void tx_released();
    void slot_changed(int);
    void cc_changed(int);
    void dmr_tgid_changed(int);
    void dmrpc_state_changed(int);
    void module_changed(char);
    void rptr1_changed(QString);
    void rptr2_changed(QString);
    void mycall_changed(QString);
    void urcall_changed(QString);
    void usrtxt_changed(QString);
    void swtx_state_changed(int);
    void swrx_state_changed(int);
    void swtx_state(int);
    void swrx_state(int);
    void dst_changed(QString);
    void debug_changed(bool);
    void m17_rate_changed(int);
    void m17_can_changed(int);
    void update_log(QString);
    void audio_ready(void);
    void update_data(void);

public slots:
    void set_callsign(const QString &callsign) {  m_callsign = callsign.simplified(); }
    void set_dmrtgid(const QString &dmrtgid) { m_dmr_destid = dmrtgid.simplified().toUInt();}
    void set_slot(const int slot) {emit slot_changed(slot); }
    void set_cc(const int cc) {emit cc_changed(cc); }
    void tgid_text_changed(QString s){emit dmr_tgid_changed(s.toUInt());}
    void set_dmrid(const QString &dmrid) { m_dmrid = dmrid.simplified().toUInt(); }
    void set_essid(const QString &essid)
    {
        if (essid != "None") {
            m_essid = essid.simplified().toUInt() + 1;
        }
        else{
            m_essid = 0;
        }
    }
    void set_bm_password(const QString &bmpwd) { m_bm_password = bmpwd; }
    void set_tgif_password(const QString &tgifpwd) { m_tgif_password = tgifpwd; }
    void set_latitude(const QString &lat){ m_latitude = lat; }
    void set_longitude(const QString &lon){ m_longitude = lon; }
    void set_location(const QString &loc){ m_location = loc; }
    void set_description(const QString &desc){ m_description = desc; }
    void set_freq(const QString &freq){ m_freq = freq; }
    void set_url(const QString &url){ m_url = url; }
    void set_swid(const QString &swid){ m_swid = swid; }
    void set_pkgid(const QString &pkgid){ m_pkgid = pkgid; }
    void set_dmr_options(const QString &dmropts) { m_dmropts = dmropts; }
    void set_dmr_pc(int pc) { m_pc = pc; emit dmrpc_state_changed(m_pc); }
    void set_reflector(const QString &host) { m_refname = host; }
    void set_module(const QString &module) { m_module = module.toStdString()[0]; emit module_changed(m_module);}
    void set_protocol(const QString &protocol) { m_protocol = protocol; }
    void set_localhosts(const QString &lhost) { m_localhosts = lhost; }
    void set_modelchange(bool t){ m_modelchange = t; }
    void set_iaxuser(const QString &user){ m_iaxuser = user; }
    void set_iaxpass(const QString &pass){ m_iaxpassword = pass; }
    void set_iaxnode(const QString &node){ m_iaxnode = node; }
    void set_iaxhost(const QString &host){ m_iaxhost = host; }
    void set_mycall(const QString &mycall) { m_mycall = mycall; emit mycall_changed(mycall); }
    void set_urcall(const QString &urcall) { m_urcall = urcall; emit urcall_changed(urcall); }
    void set_rptr1(const QString &rptr1) { m_rptr1 = rptr1; emit rptr1_changed(rptr1); qDebug() << "rpt1 == " << m_rptr1; }
    void set_rptr2(const QString &rptr2) { m_rptr2 = rptr2; emit rptr2_changed(rptr2); qDebug() << "rpt2 == " << m_rptr2; }
    void set_usrtxt(const QString &usrtxt) { m_dstarusertxt = usrtxt; emit usrtxt_changed(usrtxt); }
    void set_txtimeout(const QString &t) { m_txtimeout = t.simplified().toUInt();}
    void set_toggletx(bool x) { m_toggletx = x; }
    void set_xrf2ref(bool x) { m_xrf2ref = x; }
    void set_modemTxInvert(bool x) { m_modemTxInvert = x; }
    void set_modemRxInvert(bool x) { m_modemRxInvert = x; }
    void set_modemPTTInvert(bool x) { m_modemPTTInvert = x; }
    void set_ipv6(bool ipv6) { m_ipv6 = ipv6; }
    void set_vocoder(QString vocoder) { m_vocoder = vocoder; }
    void set_modem(QString modem) { m_modem = modem; }
    void set_swtx(bool swtx) { emit swtx_state_changed(swtx); }
    void set_swrx(bool swrx) { emit swrx_state_changed(swrx); }
 //   void set_mmdvm_direct(bool mmdvm) { m_mdirect = mmdvm; process_mode_change(m_protocol); }
    void set_iaxport(const QString &port){ m_iaxport = port.simplified().toUInt(); }
    void set_dst(QString dst){emit dst_changed(dst);}
    void set_debug(bool debug){emit debug_changed(debug);}

    void set_modemRxFreq(QString m) { m_modemRxFreq = m; }
    void set_modemTxFreq(QString m) { m_modemTxFreq = m; }
    void set_modemRxOffset(QString m) { m_modemRxOffset = m; }
    void set_modemTxOffset(QString m) { m_modemTxOffset = m; }
    void set_modemRxDCOffset(QString m) { m_modemRxDCOffset = m; }
    void set_modemTxDCOffset(QString m) { m_modemTxDCOffset = m; }
    void set_modemRxLevel(QString m) { m_modemRxLevel = m; }
    void set_modemTxLevel(QString m) { m_modemTxLevel = m; }
    void set_modemRFLevel(QString m) { m_modemRFLevel = m; }
    void set_modemTxDelay(QString m) { m_modemTxDelay = m; }
    void set_modemCWIdTxLevel(QString m) { m_modemCWIdTxLevel = m; }
    void set_modemDstarTxLevel(QString m) { m_modemDstarTxLevel = m; }
    void set_modemDMRTxLevel(QString m) { m_modemDMRTxLevel = m; }
    void set_modemYSFTxLevel(QString m) { m_modemYSFTxLevel = m; }
    void set_modemP25TxLevel(QString m) { m_modemP25TxLevel = m; }
    void set_modemNXDNTxLevel(QString m) { m_modemNXDNTxLevel = m; }
    void set_modemBaud(QString m) { m_modemBaud = m; }
    void set_modemM17CAN(QString m) { emit m17_can_changed(m.toInt()); }

    void m17_rate_changed(bool r) { emit m17_rate_changed((int)r); }
    void process_connect();
    void press_tx();
    void release_tx();
 //   void click_tx(bool);
//    void process_host_change(const QString &h);
    void dtmf_send_clicked(QString);
    bool get_modelchange(){ return m_modelchange; }
    QString get_ambestatustxt() { return m_ambestatustxt; }
    QString get_mmdvmstatustxt() { return m_mmdvmstatustxt; }
    QString get_netstatustxt() { return m_netstatustxt; }
    QString get_mode() { return m_protocol; }
    QString get_reflector() { return m_refname; }
    QString get_host() { return m_host; }
    QString get_module() { return QString(m_module); }
    QString get_callsign() { return m_callsign; }
    QString get_dmrid() { return m_dmrid ? QString::number(m_dmrid) : ""; }
    QString get_essid() { return m_essid ? QString("%1").arg(m_essid - 1, 2, 10, QChar('0')) : "None"; }
    QString get_bm_password() { return m_bm_password; }
    QString get_tgif_password() { return m_tgif_password; }
    QString get_latitude() { return m_latitude; }
    QString get_longitude() { return m_longitude; }
    QString get_location() { return m_location; }
    QString get_description() { return m_description; }
    QString get_freq() { return m_freq; }
    QString get_url() { return m_url; }
    QString get_swid() { return m_swid; }
    QString get_pkgid() { return m_pkgid; }
    QString get_dmr_options() { return m_dmropts; }
    QString get_dmrtgid() { return m_dmr_destid ? QString::number(m_dmr_destid) : ""; }
    QStringList get_hosts() { return m_hostsmodel; }
    QString get_iax_user() { return m_iaxuser; }
    QString get_iax_pass() { return m_iaxpassword; }
    QString get_iax_node() { return m_iaxnode; }
    QString get_iax_port() { return QString::number(m_iaxport); }
    QString get_mycall() { return m_mycall; }
    QString get_urcall() { return m_urcall; }
    QString get_rptr1() { return m_rptr1; }
    QString get_rptr2() { return m_rptr2; }
    QString get_dstarusertxt() { return m_dstarusertxt; }
    QString get_txtimeout() { return QString::number(m_txtimeout); }
    QString get_error_text() { return m_errortxt; }
    bool get_toggletx() { return m_toggletx; }
    bool get_ipv6() { return m_ipv6; }
    bool get_xrf2ref() { return m_xrf2ref; }
    bool get_modemTxInvert() { return m_modemTxInvert; }
    bool get_modemRxInvert() { return m_modemRxInvert; }
    bool get_modemPTTInvert() { return m_modemPTTInvert; }
    QString get_local_hosts(){ return m_localhosts; }
    QStringList get_vocoders() { return m_vocoders; }
    QStringList get_modems() { return m_modems; }
    QString get_modemRxFreq() { return m_modemRxFreq; }
    QString get_modemTxFreq() { return m_modemTxFreq; }
    QString get_modemRxOffset() { return m_modemRxOffset; }
    QString get_modemTxOffset() { return m_modemTxOffset; }
    QString get_modemRxDCOffset() { return m_modemRxDCOffset; }
    QString get_modemTxDCOffset() { return m_modemTxDCOffset; }
    QString get_modemRxLevel() { return m_modemRxLevel; }
    QString get_modemTxLevel() { return m_modemTxLevel; }
    QString get_modemRFLevel() { return m_modemRFLevel; }
    QString get_modemTxDelay() { return m_modemTxDelay; }
    QString get_modemCWIdTxLevel() { return m_modemCWIdTxLevel; }
    QString get_modemDstarTxLevel() { return m_modemDstarTxLevel; }
    QString get_modemDMRTxLevel() { return m_modemDMRTxLevel; }
    QString get_modemYSFTxLevel() { return m_modemYSFTxLevel; }
    QString get_modemP25TxLevel() { return m_modemP25TxLevel; }
    QString get_modemNXDNTxLevel() { return m_modemNXDNTxLevel; }
    QString get_modemBaud() { return m_modemBaud; }
    QString get_modemM17CAN() { return m_modemM17CAN; }
#if defined(Q_OS_ANDROID)
    QString get_platform() { return QSysInfo::productType(); }
    void reset_connect_status();
    QString get_monofont() { return "Droid Sans Mono"; }
#elif defined(Q_OS_WIN)
    QString get_platform() { return QSysInfo::kernelType(); }
    void reset_connect_status() {}
    QString get_monofont() { return "Courier"; }
#else
    QString get_platform() { return QSysInfo::kernelType(); }
    void reset_connect_status() {}
    QString get_monofont() { return "monospace"; }
#endif
    QString get_arch() { return QSysInfo::currentCpuArchitecture(); }
    QString get_build_abi() { return QSysInfo::buildAbi(); }
 //   QString get_software_build() { return VERSION_NUMBER; }

    unsigned short get_output_level(){ return m_outlevel; }
    void set_output_level(unsigned short l){ m_outlevel = l; }

private:
    int connect_status;
    bool m_update_host_files;
    QSettings *m_settings;
    QString config_path;
    QString hosts_filename;
    QString m_callsign;
    QString m_host;
    QString m_refname;
    QString m_protocol;
    QString m_bm_password;
    QString m_tgif_password;
    QString m_latitude;
    QString m_longitude;
    QString m_location;
    QString m_description;
    QString m_freq;
    QString m_url;
    QString m_swid;
    QString m_pkgid;
    QString m_dmropts;
    int m_pc;
    uint32_t m_dmrid;
    uint8_t m_essid;
    uint32_t m_dmr_srcid;
    uint32_t m_dmr_destid;
    QMap<uint32_t, QString> m_dmrids;
    QMap<uint16_t, QString> m_nxdnids;
    char m_module;
    int m_port;
    QString m_data1;
    QString m_data2;
    QString m_data3;
    QString m_data4;
    QString m_data5;
    QString m_data6;
    QString m_ambestatustxt;
    QString m_mmdvmstatustxt;
    QString m_netstatustxt;
    QString m_mycall;
    QString m_urcall;
    QString m_rptr1;
    QString m_rptr2;
    int m_txtimeout;
    bool m_toggletx;
    QString m_dstarusertxt;
    QThread *m_modethread;
    QByteArray user_data;
    QString m_iaxuser;
    QString m_iaxpassword;
    QString m_iaxnode;
    QString m_iaxhost;
    QString m_localhosts;
    int m_iaxport;
    bool m_settings_processed;
    bool m_modelchange;
    const uint8_t header[5] = {0x80,0x44,0x53,0x56,0x54}; //DVSI packet header
    uint16_t m_outlevel;
    QString m_errortxt;
    bool m_xrf2ref;
    bool m_ipv6;
    bool m_mdirect;

    int m_tts;
    QString m_ttstxt;

    QString m_modemRxFreq;
    QString m_modemTxFreq;
    QString m_modemRxOffset;
    QString m_modemTxOffset;
    QString m_modemRxDCOffset;
    QString m_modemTxDCOffset;
    QString m_modemRxLevel;
    QString m_modemTxLevel;
    QString m_modemRFLevel;
    QString m_modemTxDelay;
    QString m_modemCWIdTxLevel;
    QString m_modemDstarTxLevel;
    QString m_modemDMRTxLevel;
    QString m_modemYSFTxLevel;
    QString m_modemP25TxLevel;
    QString m_modemNXDNTxLevel;
    QString m_modemBaud;
    QString m_modemM17CAN;
    bool m_modemTxInvert;
    bool m_modemRxInvert;
    bool m_modemPTTInvert;
#ifdef Q_OS_ANDROID
    AndroidSerialPort *m_USBmonitor;
#endif

private slots:
#ifdef Q_OS_ANDROID
    void keepScreenOn();
#endif
    void tts_changed(QString);
    void tts_text_changed(QString);
    void updatelog(QString);
    void update_output_level(unsigned short l){ m_outlevel = l;}
    void process_audio(int16_t*, int);
    void update(Mode::MODEINFO);
};
#endif // DIGIHAMLIB_H
