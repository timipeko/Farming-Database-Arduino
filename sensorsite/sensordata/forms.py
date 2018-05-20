from django import forms

class MeasurementForm(forms.Form):
    sensor_id = forms.IntegerField()
    temp = forms.FloatField()
    humidity = forms.FloatField()
    status_code = forms.IntegerField()
