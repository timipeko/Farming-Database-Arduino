from django.shortcuts import render
from django.http import HttpResponse
from django.utils import timezone
from django.views.decorators.csrf import csrf_exempt

from .forms import MeasurementForm
from .models import Measurement

@csrf_exempt
def index(request):
    if request.method == 'POST':
        form = MeasurementForm(request.POST)
        # If the form is valid we can save it to the database.
        if form.is_valid():
            m = Measurement(sensor_id = form.cleaned_data['sensor_id'], 
                            date = timezone.now(), 
                            temp = form.cleaned_data['temp'], 
                            humidity = form.cleaned_data['humidity'], 
                            status_code = form.cleaned_data['status_code'])
            m.save()
            return HttpResponse("Saved measurement into database\n")
    else:
        form = MeasurementForm()
    return render(request, 'measurement.html', {'form': form})
